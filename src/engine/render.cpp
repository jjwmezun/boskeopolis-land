#include "camera.hpp"
#include <cassert>
#include <cstdio>
#include "main.hpp"
#include "render.hpp"
#include "localization.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "palette.hpp"
#include <SDL2/SDL_image.h>
#include "timers/timer_repeat.hpp"

#define magnified( n ) ( ( n ) * ( magnification_ ) )

namespace Render
{
	// Private Variables
	static constexpr Uint8 FULL_OPACITY = 255;
	static constexpr sdl2::SDLRect WINDOW_BOX = { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS };

	static Uint32 window_type_ = SDL_WINDOW_FULLSCREEN_DESKTOP;
	static int FORCE_MAGNIFICATION = -1;

	static const std::string IMG_RELATIVE_DIR = "img";

	static std::string img_path_;
	static std::map<std::string, SDL_Texture*> textures_ = {};
	static std::map<std::string, SDL_Texture*> trans_textures_ = {};
	static std::map<std::string, SDL_Surface*> surfaces_ = {};

	static SDL_Renderer* renderer_ = nullptr;
	static SDL_Window* window_ = nullptr;
	static sdl2::SDLRect screen_;
	static Palette palette_ { "Grayscale", 1 };
	static Palette main_palette_ { "Grayscale", 1 };
	static Palette trans_palette_ { "Grayscale", 1 };

	static int magnification_ = 1;
	static int max_magnification_ = 1;
	static int monitor_width_ = 0;
	static int monitor_height_ = 0;

	static SDL_Texture* current_target_ = nullptr;
	static SDL_Texture* target_ = nullptr;
	static SDL_Texture* trans_target_ = nullptr;
	static SDL_Texture* final_target_ = nullptr;

	static bool trans_ = false;
	static Uint8 trans_alpha_ = 0;


	// Private Function Declarations
	void createRenderer();
	void createWindow();

	void adjustMagnification();
	void adjustScreenToMagnification();
	int calculateMagnification( int monitor_width, int monitor_height );
	void calculateScreenEdges();

	void loadSurface( const std::string& sheet );
	void clearTextures();
	void reloadTextures();

	const std::string imgAddress( const std::string& relative_path );
	const std::string setImgPath();
	SDL_RendererFlip convertFlip( int flip_x, int flip_y );
	bool cameraAdjust( sdl2::SDLRect& dest, const Camera* camera );
	void checkTexture( const std::string& sheet );
	void setColor( int color, Uint8 alpha = 255 );

	std::map<std::string, SDL_Texture*>& getCurrentTexture();


	// Function Implementations
	void init( bool windowed, int magnification )
	{
		img_path_ = setImgPath();
		window_type_ = ( windowed ) ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_FULLSCREEN_DESKTOP;
		FORCE_MAGNIFICATION = magnification;
		adjustMagnification();
		createWindow();
		createRenderer();

		if ( !( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) )
		{
			SDL_Log( "IMG_Initialization failed: %s", SDL_GetError() );
			exit( 1 );
		}
	};

	void createRenderer()
	{
		renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED );

		if ( !renderer_ )
		{
			SDL_Log( "Renderer Initialization failed: %s", SDL_GetError() );
			exit( 1 );
		}

		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_BLEND );
		final_target_ = SDL_GetRenderTarget( renderer_ );
		target_ = SDL_CreateTexture( renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS );
		trans_target_ = SDL_CreateTexture( renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS );
		current_target_ = target_;
		SDL_SetRenderTarget( renderer_, current_target_ );
	};

	void createWindow()
	{
		window_ = SDL_CreateWindow
		(
			Localization::getGameTitle(),
			0,
			0,
			magnified( Unit::WINDOW_WIDTH_PIXELS ),
			magnified( Unit::WINDOW_HEIGHT_PIXELS ),
			window_type_
		);

		if ( !window_ )
		{
			SDL_Log( "Window Initialization failed: %s", SDL_GetError() );
			exit( 1 );
		}
	};



	/* MAGNIFICATION ADJUSTMENT
	==================================================================*/

		void adjustMagnification()
		{
			sdl2::SDLRect monitor_rect;
			bool monitor_check_code = SDL_GetDisplayBounds( 0, &monitor_rect );
			if ( monitor_check_code != 0 )
			{
				SDL_Log( "SDL_GetDisplayBounds failed: %s", SDL_GetError() );
				max_magnification_ = 1;
			}
			else
			{
				monitor_width_ = monitor_rect.w;
				monitor_height_ = monitor_rect.h;
				max_magnification_ = calculateMagnification( monitor_width_, monitor_height_ );
			}
			magnification_ = ( FORCE_MAGNIFICATION > -1 ) ? FORCE_MAGNIFICATION : max_magnification_;
			adjustScreenToMagnification();
		};

		void adjustScreenToMagnification()
		{
			if ( monitor_width_ == 0 || monitor_height_ == 0 )
			{
				window_type_ = SDL_WINDOW_RESIZABLE;
			}

			calculateScreenEdges();

			SDL_SetWindowSize
			(
				window_,
				magnified( Unit::WINDOW_WIDTH_PIXELS ),
				magnified( Unit::WINDOW_HEIGHT_PIXELS )
			);
		};

		int calculateMagnification( int monitor_width, int monitor_height )
		{
			double screen_aspect_ratio = (double)Unit::WINDOW_WIDTH_PIXELS / (double)Unit::WINDOW_HEIGHT_PIXELS;
			double monitor_aspect_ratio = (double)monitor_width / (double)monitor_height;

			if ( monitor_aspect_ratio > screen_aspect_ratio )
			{
				return floor( monitor_height / (double)Unit::WINDOW_HEIGHT_PIXELS );
			}
			else
			{
				return floor( monitor_width / (double)Unit::WINDOW_WIDTH_PIXELS );
			}
		};

		void calculateScreenEdges()
		{
			if ( window_type_ == SDL_WINDOW_RESIZABLE )
			{
				screen_.x = 0;
				screen_.y = 0;
			}
			else
			{
				screen_.x = ( monitor_width_ - magnified( Unit::WINDOW_WIDTH_PIXELS ) ) / 2;
				screen_.y = ( monitor_height_ - magnified( Unit::WINDOW_HEIGHT_PIXELS ) ) / 2;
			}
			screen_.w = magnified( Unit::WINDOW_WIDTH_PIXELS );
			screen_.h = magnified( Unit::WINDOW_HEIGHT_PIXELS );
		};



	const std::string setImgPath()
	{
		return Main::resourcePath() + IMG_RELATIVE_DIR + Main::pathDivider();
	};

	const std::string imgAddress( const std::string& relative_path )
	{
		return img_path_ + relative_path;
	};

	void loadSurface( const std::string& sheet )
	{
		SDL_RWops* rwop = SDL_RWFromFile( imgAddress( sheet ).c_str(), "rb" );

		if ( rwop == nullptr )
		{
			printf( "Failed to read file %s\n", imgAddress( sheet ).c_str() );
			SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
		}

		surfaces_.insert( std::make_pair( sheet, IMG_LoadPNG_RW( rwop ) ) );
		SDL_RWclose( rwop );

		if ( !surfaces_.at( sheet ) )
		{
			printf( "Failed to make surface for %s\n", imgAddress( sheet ).c_str() );
			SDL_Log( "SDL_Surface failed: %s", SDL_GetError() );
		}
	};

	void loadTexture( const std::string& sheet )
	{
		if ( mezun::notInMap( surfaces_, sheet ) )
		{
			loadSurface( sheet );
		}

		if ( surfaces_.at( sheet ) != nullptr )
		{
			palette_.applyPalette( surfaces_.at( sheet ) );

			getCurrentTexture().insert
			(
				std::make_pair
				(
					sheet,
					SDL_CreateTextureFromSurface( renderer_, surfaces_.at( sheet ) )
				)
			);

			if ( getCurrentTexture().at( sheet ) == nullptr )
			{
				SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
			}
		}
	};

	void clearTextures()
	{
		for ( auto& t : textures_ )
		{
			if ( t.second != nullptr )
			{
				SDL_DestroyTexture( t.second );
				t.second = nullptr;
			}
		}

		for ( auto& t : trans_textures_ )
		{
			if ( t.second != nullptr )
			{
				SDL_DestroyTexture( t.second );
				t.second = nullptr;
			}
		}

		textures_.clear();
		trans_textures_.clear();
	};

	void clearSurfaces()
	{
		clearTextures();

		for ( auto& s : surfaces_ )
		{
			if ( s.second != nullptr )
			{
				SDL_FreeSurface( s.second );
				s.second = nullptr;
			}
		}
		surfaces_.clear();
	};

	void renderTarget()
	{
		SDL_SetRenderTarget( renderer_, final_target_ );
		if ( current_target_ == trans_target_ )
		{
			SDL_SetTextureBlendMode( current_target_, SDL_BLENDMODE_BLEND );
			SDL_SetTextureAlphaMod( current_target_, trans_alpha_ );
		}
		if ( SDL_RenderCopy( renderer_, current_target_, &WINDOW_BOX, &screen_ ) != 0 )
		{
			printf( "Failed to render final texture: %s\n", SDL_GetError() );
		}
	}

	void setTargetToCurrent()
	{
		SDL_SetRenderTarget( renderer_, current_target_ );
	}

	void presentScreen()
	{
		SDL_RenderPresent( renderer_ );
	};

	// To prevent weird fade conflicts with palettes that replace the color white,
	// this forces white, e'en on those palettes.
	void stateChangeFade( int alpha )
	{
		SDL_SetRenderDrawColor( renderer_, 255, 255, 255, alpha );
		SDL_RenderFillRect( renderer_, &screen_ );
	};

	void colorCanvas( int color, int alpha )
	{
		const SDL_Color color_obj = palette_.color( color );
		colorCanvasForceColor( color_obj.r, color_obj.g, color_obj.b, alpha );
	};

	void colorCanvas()
	{
		colorCanvas( palette_.bgN() );
	};

	void colorCanvasForceColor( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha )
	{
		SDL_SetRenderDrawColor( renderer_, r, g, b, alpha );
		SDL_RenderFillRect( renderer_, &WINDOW_BOX );
	};

	void colorCanvasMultiply( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha )
	{
		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_MOD );
		colorCanvasForceColor( r, g, b, alpha );
		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_BLEND );
	}

	void colorCanvasBlend( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha, SDL_BlendMode blend_mode )
	{
		SDL_SetRenderDrawBlendMode( renderer_, blend_mode );
		colorCanvasForceColor( r, g, b, alpha );
		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_BLEND );
	}

	void clearScreen()
	{
		SDL_SetRenderTarget( renderer_, final_target_ );
		SDL_SetRenderDrawColor( renderer_, 0, 0, 0, FULL_OPACITY );
		SDL_RenderClear( renderer_ );
		SDL_SetRenderTarget( renderer_, current_target_ );
	};

	void clearRenderBox( SDL_Texture* texture )
	{
		setRenderTarget( texture );
		clearScreenTransparency();
		releaseRenderTarget();
	};

	void clearScreenTransparency()
	{
		SDL_SetRenderDrawColor( renderer_, 0, 0, 0, 0 );
		SDL_RenderClear( renderer_ );
	};

	SDL_RendererFlip convertFlip( int flip_x, int flip_y )
	{
		if ( flip_x && flip_y )
		{
			return ( SDL_RendererFlip )( SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL );
		}
		else if ( flip_x )
		{
			return SDL_FLIP_HORIZONTAL;
		}
		else if ( flip_y )
		{
			return SDL_FLIP_VERTICAL;
		}
		else
		{
			return SDL_FLIP_NONE;
		}
	};

	void renderObject
	(
		const std::string& sheet,
		const sdl2::SDLRect& source,
		const sdl2::SDLRect& dest,
		bool flip_x,
		bool flip_y,
		double rotation,
		Uint8 alpha,
		const Camera* camera,
		SDL_BlendMode blend_mode,
		SDL_Texture* alt_texture,
		const Point* rotation_center
	)
	{
		renderObject( sheet, source, dest, convertFlip( flip_x, flip_y ), rotation, alpha, camera, blend_mode, alt_texture, rotation_center );
	};

	void renderObject
	(
		const std::string& sheet,
		sdl2::SDLRect source,
		sdl2::SDLRect dest,
		SDL_RendererFlip flip,
		double rotation,
		Uint8 alpha,
		const Camera* camera,
		SDL_BlendMode blend_mode,
		SDL_Texture* alt_texture,
		const Point* rotation_center
	)
	{
		checkTexture( sheet );
		renderObject( getCurrentTexture().at( sheet ), source, dest, flip, rotation, alpha, camera, blend_mode, alt_texture, rotation_center );
	}

	void renderObject
	(
		SDL_Texture* texture,
		sdl2::SDLRect source,
		sdl2::SDLRect dest,
		SDL_RendererFlip flip,
		double rotation,
		Uint8 alpha,
		const Camera* camera,
		SDL_BlendMode blend_mode,
		SDL_Texture* alt_texture,
		const Point* rotation_center
	)
	{
		if ( alt_texture != nullptr )
		{
			SDL_SetRenderTarget( renderer_, alt_texture );
			if ( SDL_RenderCopyEx( renderer_, texture, &source, &dest, rotation, rotation_center, flip ) != 0 )
			{
				printf( "Render failure: %s\n", SDL_GetError() );
			}
			SDL_SetRenderTarget( renderer_, current_target_ );
		}
		else
		{
			SDL_SetTextureAlphaMod( texture, alpha );

			if ( !cameraAdjust( dest, camera ) )
			{
				return;
			};

			if ( blend_mode != SDL_BLENDMODE_BLEND && blend_mode != SDL_BLENDMODE_NONE )
			{
				SDL_SetRenderDrawBlendMode( renderer_, blend_mode );
				SDL_SetTextureBlendMode( texture, blend_mode );
			}
			
			if ( SDL_RenderCopyEx( renderer_, texture, &source, &dest, rotation, rotation_center, flip ) != 0 )
			{
				printf( "Render failure: %s\n", SDL_GetError() );
			}

			if ( blend_mode != SDL_BLENDMODE_BLEND )
			{
				SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_BLEND );
			}
		}
	};

	void checkTexture( const std::string& sheet )
	{
		if ( getCurrentTexture().find( sheet ) == getCurrentTexture().end() )
		{
			loadTexture( sheet );
		}
	};

	bool cameraAdjust( sdl2::SDLRect& dest, const Camera* camera )
	{
		if ( camera != nullptr )
		{
			if ( camera->onscreenPixels( dest ) )
			{
				dest.x = camera->relativeX( dest );
				dest.y = camera->relativeY( dest );
				return true;
			}
			else
			{
				return false; // If not onscreen, don't draw; just quit function now.
			}
		}
		else
		{
			return true;
		}
	};

	void setColor( int c, Uint8 alpha )
	{
		const sdl2::SDLColor color = palette_.color( c );
		const Uint8 r = color.r;
		const Uint8 g = color.g;
		const Uint8 b = color.b;
		SDL_SetRenderDrawColor( renderer_, r, g, b, alpha );
	};

	void renderRect( const sdl2::SDLRect& box, int color, int alpha )
	{
		setColor( color, alpha );
		SDL_RenderFillRect( renderer_, &box );
	};

	void renderLine( int x1, int y1, int x2, int y2, int color, int alpha )
	{
		setColor( color, alpha );
		SDL_RenderDrawLine( renderer_, x1, y1, x2, y2 );
	};

	void renderLine( Line line, int color, int alpha )
	{
		renderLine( line.p1.x, line.p1.y, line.p2.x, line.p2.y, color, alpha );
	}

	void renderLineDebug( Line line, SDL_Color color )
	{
		SDL_SetRenderDrawColor( renderer_, color.r, color.g, color.b, color.a );
		SDL_RenderDrawLine( renderer_, line.p1.x, line.p1.y, line.p2.x, line.p2.y );
	};

	void renderRectDebug( const sdl2::SDLRect& box, SDL_Color color )
	{
		SDL_SetRenderDrawColor( renderer_, color.r, color.g, color.b, color.a );
		SDL_RenderFillRect( renderer_, &box );
	};

	void renderRectCamera( sdl2::SDLRect box, const Camera& camera, int color, int alpha )
	{
		cameraAdjust( box, &camera );
		renderRect( box, color, alpha );
	};

	void renderPointDebug( const Point& point, SDL_Color color )
	{
		SDL_SetRenderDrawColor( renderer_, color.r, color.g, color.b, color.a );
		const sdl2::SDLRect box = { point.x, point.y, 1, 1 };
		SDL_RenderFillRect( renderer_, &box );
	};

	void newPalette( Palette palette )
	{
		if ( main_palette_ != palette )
		{
			palette_ = main_palette_ = palette;
			trans_ = false;
			current_target_ = target_;
			clearTextures();
		}
	};

	void quit()
	{
		SDL_DestroyTexture( target_ );
		SDL_DestroyTexture( trans_target_ );
		clearSurfaces();
		IMG_Quit();
		SDL_DestroyRenderer( renderer_ );
		SDL_DestroyWindow( window_ );
	};

	SDL_Texture* createRenderBox( int width, int height )
	{
		SDL_Texture* temp = SDL_CreateTexture( renderer_, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, width, height );
		if ( temp == nullptr )
		{
			printf( "Failed to allocate render box: %s\n", SDL_GetError() );
		}
		SDL_SetTextureBlendMode( temp, SDL_BLENDMODE_BLEND );
		return temp;
	};

	// Used to start generating textures.
	// When this is set, all drawing will be done on user-given texture
	// 'stead o' screen texture used for main drawing.
	void setRenderTarget( SDL_Texture* texture )
	{
		SDL_SetRenderTarget( renderer_, texture );
	};

	// Make sure this is called when done rendering to custom texture
	// or game rendering will glitch up for a frame
	// & the custom texture will be corrupted by all other game graphics.
	void releaseRenderTarget()
	{
		SDL_SetRenderTarget( renderer_, current_target_ );
	};

	void renderRenderBox( SDL_Texture* texture )
	{
		if ( SDL_RenderCopy( renderer_, texture, &WINDOW_BOX, &WINDOW_BOX ) )
		{
			printf( "Failed to draw render box: %s\n", SDL_GetError() );
		}
	};

	void renderRenderBox( SDL_Texture* texture, sdl2::SDLRect src )
	{
		if ( SDL_RenderCopy( renderer_, texture, &src, &src ) )
		{
			printf( "Failed to draw render box: %s\n", SDL_GetError() );
		}
	};

	void renderRenderBox( SDL_Texture* texture, sdl2::SDLRect src, sdl2::SDLRect dest )
	{
		if ( SDL_RenderCopy( renderer_, texture, &src, &dest ) )
		{
			printf( "Failed to draw render box: %s\n", SDL_GetError() );
		}
	};

	int getMaxMagnification()
	{
		return max_magnification_;
	};

	int getMagnification()
	{
		return magnification_;
	};

	bool isFullscreen()
	{
		return window_type_ == SDL_WINDOW_FULLSCREEN_DESKTOP;
	};

	void setWindowed()
	{
		window_type_ = SDL_WINDOW_RESIZABLE;
		SDL_SetWindowFullscreen( window_, 0 );
		adjustScreenToMagnification();
	};

	void setFullscreen()
	{
		window_type_ = SDL_WINDOW_FULLSCREEN_DESKTOP;
		SDL_SetWindowFullscreen( window_, SDL_WINDOW_FULLSCREEN_DESKTOP );
		adjustScreenToMagnification();
	};

	void setMagnification( int magnification )
	{
		magnification_ = magnification;
		adjustScreenToMagnification();
	};

	const sdl2::SDLRect& getScreen()
	{
		return screen_;
	};

	SDL_Surface* getSurface( std::string sheet )
	{
		checkTexture( sheet );
		return surfaces_.at( sheet );
	};

	void tintScreen( const SDL_Color& color )
	{
		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_ADD );
		colorCanvasForceColor( color.r, color.g, color.b, color.a );		
		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_BLEND );
	};

	void turnOnTransitionPalette( Palette pal1, Palette pal2 )
	{
		clearTextures();
		trans_ = true;
		palette_ = main_palette_ = pal1;
		trans_palette_ = pal2;
	};

	void setPaletteTransAlpha( Uint8 alpha )
	{
		trans_alpha_ = alpha;
	};

	bool hasTransPalette()
	{
		return trans_;
	};

	void swapTransPalette()
	{
		current_target_ = ( current_target_ == target_ ) ? trans_target_ : target_;
		palette_ = ( current_target_ == target_ ) ? main_palette_ : trans_palette_;
	}

	std::map<std::string, SDL_Texture*>& getCurrentTexture()
	{
		return ( current_target_ == target_ ) ? textures_ : trans_textures_;
	};
};
