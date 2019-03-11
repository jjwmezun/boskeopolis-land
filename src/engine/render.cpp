#include "camera.hpp"
#include <cassert>
#include <cstdio>
#include "main.hpp"
#include "render.hpp"
#include <iostream>
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "palette.hpp"
#include <SDL2/SDL_image.h>
#include "timers/timer_repeat.hpp"

namespace Render
{
	// Private Variables
	static constexpr Uint8 FULL_OPACITY = 255;
	static sdl2::SDLRect window_box_magnified_ = { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS };

	static Uint32 window_type_ = SDL_WINDOW_FULLSCREEN_DESKTOP;
	static int FORCE_MAGNIFICATION = -1;

	static const std::string IMG_RELATIVE_DIR = "img";

	static std::string img_path_;
	static std::map<std::string, SDL_Texture*> textures_ = {};
	static std::map<std::string, SDL_Surface*> surfaces_ = {};

	static std::vector<sdl2::SDLRect> rect_layer_1_;
	static std::vector<sdl2::SDLRect> rect_layer_2_;
	static std::vector<sdl2::SDLRect> rect_layer_3_;

	static SDL_Renderer* renderer_ = nullptr;
	static SDL_Window* window_ = nullptr;
	static sdl2::SDLRect screen_;
	static TimerRepeat animation_frame_ = {};
	static std::unique_ptr<const Palette> palette_ = std::make_unique<Palette> ( "Grayscale", 1 );

	static int magnification_ = 1;
	static int max_magnification_ = 1;
	static int monitor_width_ = 0;
	static int monitor_height_ = 0;
	static sdl2::SDLRect top_bar_ = { 0, 0, 0, 0 };
	static sdl2::SDLRect bottom_bar_ = { 0, 0, 0, 0 };
	static sdl2::SDLRect left_bar_ = { 0, 0, 0, 0 };
	static sdl2::SDLRect right_bar_ = { 0, 0, 0, 0 };

	static SDL_Texture* target_ = nullptr;


	// Private Function Declarations
	void createRenderer();
	void createWindow();

	int magnified( int n );
	void adjustMagnification();
	void adjustScreenToMagnification();
	int calculateMagnification( int monitor_width, int monitor_height );
	void calculateScreenEdges();
	void adjustBorderBars();

	void loadSurface( const std::string& sheet );
	void clearTextures();
	void reloadTextures();

	const std::string imgAddress( const std::string& relative_path );
	const std::string setImgPath();
	sdl2::SDLRect sourceRelativeToScreen( const sdl2::SDLRect& source );
	SDL_RendererFlip convertFlip( int flip_x, int flip_y );
	bool cameraAdjust( sdl2::SDLRect& dest, const Camera* camera );
	void checkTexture( const std::string& sheet );


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
		renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_TARGETTEXTURE );

		if ( !renderer_ )
		{
			SDL_Log( "Renderer Initialization failed: %s", SDL_GetError() );
			exit( 1 );
		}

		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_BLEND );
		target_ = SDL_GetRenderTarget( renderer_ );
	};

	void createWindow()
	{
		window_ = SDL_CreateWindow
		(
			"Boskeopolis Land",
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
			adjustBorderBars();
			window_box_magnified_ = sourceRelativeToScreen( window_box_ );

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

		void adjustBorderBars()
		{
			top_bar_ = { 0, 0, monitor_width_, screen_.y };
			bottom_bar_ = { 0, screen_.y + screen_.h, monitor_width_, monitor_height_ - ( screen_.y + screen_.h ) };
			left_bar_ = { 0, 0, screen_.x, monitor_height_ };
			right_bar_ = { screen_.x + screen_.w, 0, monitor_width_ - ( screen_.x + screen_.w ), monitor_height_ };
		};

		int magnified( int n )
		{
			return n * magnification_;
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
			SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
		}

		surfaces_.insert( std::make_pair( sheet, IMG_LoadPNG_RW( rwop ) ) );
		SDL_RWclose( rwop );

		if ( !surfaces_.at( sheet ) )
		{
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
			if ( palette_ )
			{
				palette_->applyPalette( surfaces_.at( sheet ) );
			}

			textures_.insert
			(
				std::make_pair
				(
					sheet,
					SDL_CreateTextureFromSurface( renderer_, surfaces_.at( sheet ) )
				)
			);

			if ( textures_.at( sheet ) == nullptr )
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

		textures_.clear();
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

	void screenBorders()
	{
		if ( window_type_ == SDL_WINDOW_FULLSCREEN_DESKTOP )
		{
			SDL_SetRenderDrawColor( renderer_, 0, 0, 0, FULL_OPACITY );
			SDL_RenderFillRect( renderer_, &top_bar_ );
			SDL_RenderFillRect( renderer_, &bottom_bar_ );
			SDL_RenderFillRect( renderer_, &left_bar_ );
			SDL_RenderFillRect( renderer_, &right_bar_ );
		}
	};

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
		assert( palette_ );
		const SDL_Color color_obj = palette_->color( color );
		colorCanvasForceColor( color_obj.r, color_obj.g, color_obj.b, alpha );
	};

	void colorCanvas()
	{
		colorCanvas( palette_->bgN() );
	};

	void colorCanvasForceColor( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha )
	{
		SDL_SetRenderDrawColor( renderer_, r, g, b, alpha );
		SDL_RenderFillRect( renderer_, &screen_ );
	};

	void colorCanvasMultiply( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha )
	{
		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_MOD );
		colorCanvasForceColor( r, g, b, alpha );
		SDL_SetRenderDrawBlendMode( renderer_, SDL_BLENDMODE_BLEND );
	}

	void clearScreen()
	{
		SDL_SetRenderDrawColor( renderer_, 0, 0, 0, FULL_OPACITY );
		SDL_RenderClear( renderer_ );
	};

	void clearScreenTransparency()
	{
		SDL_SetRenderDrawColor( renderer_, 0, 0, 0, 0 );
		SDL_RenderClear( renderer_ );
	};

	sdl2::SDLRect sourceRelativeToScreen( const sdl2::SDLRect& source )
	{
		return
		{
			magnified( source.x ) + screen_.x,
			magnified( source.y ) + screen_.y,
			magnified( source.w ),
			magnified( source.h ),
		};
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
		SDL_Texture* alt_texture
	)
	{
		renderObject( sheet, source, dest, convertFlip( flip_x, flip_y ), rotation, alpha, camera, blend_mode, alt_texture );
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
		SDL_Texture* alt_texture
	)
	{
		checkTexture( sheet );
		renderObject( textures_.at( sheet ), source, dest, flip, rotation, alpha, camera, blend_mode, alt_texture );
	}

	void renderObjectNoMagnify
	(
		const std::string& sheet,
		sdl2::SDLRect source,
		sdl2::SDLRect dest
	)
	{
		checkTexture( sheet );
		SDL_Texture* texture = textures_.at( sheet );

		if ( SDL_RenderCopy( renderer_, texture, &source, &dest ) != 0 )
		{
			printf( "Failure to render\"%s\": %s\n", sheet.c_str(), SDL_GetError() );
		}
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
		SDL_Texture* alt_texture
	)
	{
		if ( alt_texture != nullptr )
		{
			SDL_SetRenderTarget( renderer_, alt_texture );
			if ( SDL_RenderCopyEx( renderer_, texture, &source, &dest, rotation, 0, flip ) != 0 )
			{
				printf( "Render failure: %s\n", SDL_GetError() );
			}
			SDL_SetRenderTarget( renderer_, target_ );
		}
		else
		{
			SDL_SetTextureAlphaMod( texture, alpha );

			if ( !cameraAdjust( dest, camera ) )
			{
				return;
			};

			dest = sourceRelativeToScreen( dest );

			if ( blend_mode != SDL_BLENDMODE_BLEND && blend_mode != SDL_BLENDMODE_NONE )
			{
				SDL_SetRenderDrawBlendMode( renderer_, blend_mode );
				SDL_SetTextureBlendMode( texture, blend_mode );
			}

			if ( SDL_RenderCopyEx( renderer_, texture, &source, &dest, rotation, 0, flip ) != 0 )
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
		if ( textures_.find( sheet ) == textures_.end() )
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

	void renderRect( const sdl2::SDLRect& box, int color, int alpha )
	{
		assert( palette_ );

		const Uint8 r = palette_->color( color ).r;
		const Uint8 g = palette_->color( color ).g;
		const Uint8 b = palette_->color( color ).b;

		sdl2::SDLRect box_relative = sourceRelativeToScreen( box );
		SDL_SetRenderDrawColor( renderer_, r, g, b, alpha );
		SDL_RenderFillRect( renderer_, &box_relative );
	};

	void renderRectDebug( const sdl2::SDLRect& box, SDL_Color color )
	{
		sdl2::SDLRect box_relative = sourceRelativeToScreen( box );
		SDL_SetRenderDrawColor( renderer_, color.r, color.g, color.b, color.a );
		SDL_RenderFillRect( renderer_, &box_relative );
	};

	void renderRectCamera( sdl2::SDLRect& box, const Camera& camera, int color, int alpha )
	{
		cameraAdjust( box, &camera );
		renderRect( box, color, alpha );
	};

	void newPalette( Palette palette )
	{
		palette_ = std::make_unique<Palette> ( palette );
		clearTextures();
	};

	void quit()
	{
		clearSurfaces();
		IMG_Quit();
		SDL_DestroyRenderer( renderer_ );
		SDL_DestroyWindow( window_ );
	};

	SDL_Texture* createRenderBoxMagnified( int width, int height )
	{
		return createRenderBox( magnified( width ), magnified( height ) );
	};

	SDL_Texture* createRenderBox( int width, int height )
	{
		SDL_Texture* temp = SDL_CreateTexture( renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height );

		if ( temp == nullptr )
		{
			printf( "Failed to allocate render box: %s\n", SDL_GetError() );
		}

		SDL_SetTextureBlendMode( temp, SDL_BLENDMODE_BLEND );

		return temp;
	};

	void setRenderTarget( SDL_Texture* texture )
	{
		SDL_SetRenderTarget( renderer_, texture );
	};

	void releaseRenderTarget()
	{
		SDL_SetRenderTarget( renderer_, target_ );
	};

	void renderRenderBox( SDL_Texture* texture )
	{
		if ( SDL_RenderCopy( renderer_, texture, &window_box_magnified_, &window_box_magnified_ ) )
		{
			printf( "Failed to draw render box: %s\n", SDL_GetError() );
		}
	};

	void renderRenderBox( SDL_Texture* texture, sdl2::SDLRect src )
	{
		const sdl2::SDLRect dest = sourceRelativeToScreen( src );
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

	SDL_Texture* createTexture()
	{
		return SDL_CreateTexture( renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screen_.w, screen_.h );
	}
};
