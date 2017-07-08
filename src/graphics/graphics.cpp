

// Name
//===================================
//
// Graphics
//


// DEPENDENCIES
//===================================

	#include <cassert>
    #include "game.h"
    #include "graphics.h"
	#include "mezun_exceptions.h"
	#include "mezun_helpers.h"
    #include <SDL2/SDL_image.h>
    #include "unit.h"


// STATIC PROPERTIES
//===================================

    Uint32 Graphics::WINDOW_TYPE = SDL_WINDOW_FULLSCREEN_DESKTOP;
    int Graphics::FORCE_MAGNIFICATION = -1;
    TimerRepeat Graphics::animation_frame_ = TimerRepeat();


// METHODS
//===================================

    Graphics::Graphics( Game& game, const std::vector<std::string>& args )
    :
        img_path_ ( setImgPath( game ) ),
        palette_ ( std::make_unique<Palette> ( "Grayscale", 1 ) )
    {
		consoleArguments( args );
        adjustMagnification();
        createWindow();
        createRenderer();
    };

    Graphics::~Graphics()
    {
        IMG_Quit();
    };

	void Graphics::consoleArguments( const std::vector<std::string>& args )
	{
        if ( args.size() >= 2 )
        {
            if ( args.at( 1 ) == "full" )
            {
                WINDOW_TYPE = SDL_WINDOW_FULLSCREEN_DESKTOP;
            }
            else if ( args.at( 1 ) == "window" )
            {
                WINDOW_TYPE = SDL_WINDOW_RESIZABLE;
            }
            else
            {
                std::cout<<"ERROR: Invalid 2nd argument. (Should be \"full\" or \"window\".";
            }
        }

        if ( args.size() >= 3 )
        {
            int mag_size = std::stoi( args.at( 2 ) );
            FORCE_MAGNIFICATION = std::stoi( args.at( 2 ) );
        }
	};

    void Graphics::createRenderer()
    {
        renderer_.reset( SDL_CreateRenderer( window_.get(), -1, SDL_RENDERER_ACCELERATED ) );

        if ( !renderer_ )
        {
            throw mezun::SDLRendererFailure();
        }
		
		SDL_SetRenderDrawBlendMode( renderer_.get(), SDL_BLENDMODE_BLEND );
    };

    void Graphics::createWindow()
    {
        window_.reset
		(
			SDL_CreateWindow
			(
				"Boskeopolis Land",
				0,
				0,
				magnified( Unit::WINDOW_WIDTH_PIXELS ),
				magnified( Unit::WINDOW_HEIGHT_PIXELS ),
				WINDOW_TYPE
			)
		);

        if ( !window_ )
        {
			throw mezun::SDLWindowFailure();
        }
    };



	/* MAGNIFICATION ADJUSTMENT
	==================================================================*/

		void Graphics::adjustMagnification()
		{
			int monitor_width = Unit::WINDOW_WIDTH_PIXELS;
			int monitor_height = Unit::WINDOW_HEIGHT_PIXELS;

			sdl2::SDLRect r;
			if ( FORCE_MAGNIFICATION > -1 )
			{
				magnification_ = FORCE_MAGNIFICATION;
			}
			else if ( SDL_GetDisplayBounds( 0, &r ) != 0 )
			{
				SDL_Log( "SDL_GetDisplayBounds failed: %s", SDL_GetError() );
				magnification_ = 1;
			}
			else
			{
				monitor_width = r.w;
				monitor_height = r.h;

				magnification_ = calculateMagnification( monitor_width, monitor_height );
			}

			adjustScreen( monitor_width, monitor_height );
			adjustBorderBars( monitor_width, monitor_height );

			SDL_SetRenderTarget( renderer_.get(), screen_surface_.get() );
		};

		int Graphics::calculateMagnification( int monitor_width, int monitor_height )
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

		void Graphics::adjustScreen( int monitor_width, int monitor_height )
		{
			calculateScreenEdges( monitor_width, monitor_height );

			screen_.x = left_edge_;
			screen_.y = top_edge_;
			screen_.w = magnified( Unit::WINDOW_WIDTH_PIXELS );
			screen_.h = magnified( Unit::WINDOW_HEIGHT_PIXELS );
		};

		void Graphics::calculateScreenEdges( int monitor_width, int monitor_height )
		{
			if ( WINDOW_TYPE == SDL_WINDOW_RESIZABLE )
			{
				left_edge_ = 0;
				top_edge_ = 0;
			}
			else
			{
				left_edge_ = ( monitor_width - magnified( Unit::WINDOW_WIDTH_PIXELS ) ) / 2;
				top_edge_ = ( monitor_height - magnified( Unit::WINDOW_HEIGHT_PIXELS ) ) / 2;
			}
		};

		void Graphics::adjustBorderBars( int monitor_width, int monitor_height )
		{
			top_bar_ = { 0, 0, monitor_width, top_edge_ };
			bottom_bar_ = { 0, screen_.y + screen_.h, monitor_width, monitor_height - ( screen_.y + screen_.h ) };
			left_bar_ = { 0, 0, left_edge_, monitor_height };
			right_bar_ = { screen_.x + screen_.w, 0, monitor_width - ( screen_.x + screen_.w ), monitor_height };
		};

		int Graphics::magnified( int n ) const
		{
			return n * magnification_;
		};



    const std::string Graphics::setImgPath( Game& game )
    {
        return game.resourcePath() + IMG_RELATIVE_DIR + game.pathDivider();
    };

    const std::string Graphics::imgAddress( const std::string& relative_path ) const
    {
        return img_path_ + relative_path;
    };

	void Graphics::loadSurface( const std::string& sheet )
	{
		SDL_RWops* rwop = SDL_RWFromFile( imgAddress( sheet ).c_str(), "rb" );

		if ( rwop == nullptr )
		{
			SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
		}

		surfaces_.insert( std::make_pair( sheet, sdl2::SDLSurface( IMG_LoadPNG_RW( rwop ) ) ) );

		if ( !surfaces_.at( sheet ) )
		{
			SDL_Log( "SDL_Surface failed: %s", SDL_GetError() );
		}
	};

    void Graphics::loadTexture( const std::string& sheet )
    {
		if ( mezun::notInMap( surfaces_, sheet ) )
		{
			loadSurface( sheet );
		}

		if ( surfaces_.at( sheet ) )
		{
			if ( palette_ )
			{
				palette_->applyPalette( surfaces_.at( sheet ).get() );
			}

			textures_.insert
			(
				std::make_pair
				(
					sheet,
					sdl2::SDLTexture
					(
						SDL_CreateTextureFromSurface( renderer_.get(), surfaces_.at( sheet ).get() )
					)
				)
			);

			if ( textures_.at( sheet ).get() == nullptr )
			{
				SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
			}
		}
    };

    void Graphics::clearTextures()
    {
        textures_.clear();
    };

    void Graphics::clearSurfaces()
    {
        clearTextures();
        surfaces_.clear();
    };

    void Graphics::screenBorders()
    {
        if ( WINDOW_TYPE == SDL_WINDOW_FULLSCREEN_DESKTOP )
        {
            SDL_SetRenderDrawColor( renderer_.get(), 0, 0, 0, FULL_OPACITY );
            SDL_RenderFillRect( renderer_.get(), &top_bar_ );
            SDL_RenderFillRect( renderer_.get(), &bottom_bar_ );
            SDL_RenderFillRect( renderer_.get(), &left_bar_ );
            SDL_RenderFillRect( renderer_.get(), &right_bar_ );
        }
    };

    void Graphics::presentScreen()
    {
        SDL_RenderPresent( renderer_.get() );
    };

    void Graphics::colorCanvas()
    {
		assert( palette_ );
		
		const Uint8 r = palette_->bgR();
		const Uint8 g = palette_->bgG();
		const Uint8 b = palette_->bgB();

        SDL_SetRenderDrawColor( renderer_.get(), r, g, b, FULL_OPACITY );
        SDL_RenderFillRect( renderer_.get(), &screen_ );
    };

    void Graphics::clearScreen()
    {
        SDL_SetRenderDrawColor( renderer_.get(), 0, 0, 0, FULL_OPACITY );
        SDL_RenderClear( renderer_.get() );
    };

    sdl2::SDLRect Graphics::sourceRelativeToScreen( const sdl2::SDLRect& source ) const
    {
        return
        {
            magnified( source.x ) + left_edge_,
            magnified( source.y ) + top_edge_,
            magnified( source.w ),
            magnified( source.h ),
        };
    };

    void Graphics::renderObject( const std::string& sheet, sdl2::SDLRect source, sdl2::SDLRect dest, SDL_RendererFlip flip, double rotation )
    {
		if ( textures_.find( sheet ) == textures_.end() )
		{
			loadTexture( sheet );
		}

		dest = sourceRelativeToScreen( dest );
		
		if ( SDL_RenderCopyEx( renderer_.get(), textures_.at( sheet ).get(), &source, &dest, rotation, 0, flip ) != 0 )
			printf( "Render failure: %s\n", SDL_GetError() );
    };

    void Graphics::renderRect( const sdl2::SDLRect& box, int color, int alpha )
    {
		assert( palette_ );

		const Uint8 r = palette_->color( color ).r;
		const Uint8 g = palette_->color( color ).g;
		const Uint8 b = palette_->color( color ).b;

        sdl2::SDLRect box_relative = sourceRelativeToScreen( box );
        SDL_SetRenderDrawColor( renderer_.get(), r, g, b, alpha );
        SDL_RenderFillRect( renderer_.get(), &box_relative );
    };

    void Graphics::newPalette( Palette palette )
    {
        palette_ = std::make_unique<Palette> ( palette );
        clearTextures();
    };