

// Name
//===================================
//
// Graphics
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "graphics.h"
    #include <SDL2/SDL_image.h>
    #include "unit.h"


// STATIC PROPERTIES
//===================================

    Uint32 Graphics::WINDOW_TYPE = SDL_WINDOW_FULLSCREEN_DESKTOP;
    int Graphics::FORCE_MAGNIFICATION = -1;
    TimerRepeat Graphics::animation_frame_ = TimerRepeat();


// METHODS
//===================================

    Graphics::Graphics( Game& game, std::vector<std::string>& args  )
    :
        img_path_ ( setImgPath( game ) ),
        palette_ ( new Palette() )
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

        adjustMagnification();
        createWindow();
        createRenderer();
    };

    Graphics::~Graphics()
    {
        IMG_Quit();
    };

    const std::string Graphics::imgAddress( const std::string& relative_path ) const
    {
        return img_path_ + relative_path;
    };

    void Graphics::loadTexture( SpriteSheet sheet )
    {
        if ( textures_.find( sheet ) == textures_.end() )
        {
            SDL_RWops* rwop = SDL_RWFromFile( imgAddress( graphic_addresses_[ (int)sheet ] ).c_str(), "rb" );

            if ( rwop == nullptr )
            {
                SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
            }

            if ( surfaces_.find( sheet ) == surfaces_.end() )
            {
                surfaces_.insert( std::make_pair( sheet, sdl2::SDLSurface( IMG_LoadPNG_RW( rwop ) ) ) );

                if ( surfaces_.at( sheet ) == nullptr )
                {
                    SDL_Log( "SDL_Surface failed: %s", SDL_GetError() );
                }
            }

            if ( surfaces_.at( sheet ) != nullptr )
            {
                if ( palette_ != nullptr )
                    palette_->applyPalette( surfaces_.at( sheet ).get() );

                textures_.insert( std::make_pair( sheet, sdl2::SDLTexture( SDL_CreateTextureFromSurface( renderer_.get(), surfaces_.at( sheet ).get() ) ) ) );

                if ( textures_.at( sheet ).get() == nullptr )
                {
                    SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
                }
            }
        }
    };

    bool Graphics::createRenderer()
    {
        if ( window_ == nullptr )
            return false;

        renderer_.reset( SDL_CreateRenderer( window_.get(), -1, SDL_RENDERER_ACCELERATED ) );

        if ( renderer_ == nullptr )
        {
            SDL_Log( "SDL_CreateRenderer failed: %s", SDL_GetError() );
            return false;
        }

        return true;
    };

    bool Graphics::createWindow()
    {
        window_.reset( SDL_CreateWindow
        (
            "Boskeopolis Land",
            0,
            0,
            magnified( Unit::WINDOW_WIDTH_PIXELS ),
            magnified( Unit::WINDOW_HEIGHT_PIXELS ),
            WINDOW_TYPE
            //SDL_WINDOW_RESIZABLE
            //SDL_WINDOW_FULLSCREEN_DESKTOP
        ) );

        if ( window_ == nullptr )
        {
            SDL_Log( "SDL_WindowCreation failed: %s", SDL_GetError() );
            return false;
        }

        return true;
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

    void Graphics::colorCanvas()
    {
        int r = 255;
        int g = 255;
        int b = 255;

        if ( palette_.get() != nullptr )
        {
            r = palette_->bgR();
            g = palette_->bgG();
            b = palette_->bgB();
        }

        SDL_SetRenderDrawColor( renderer_.get(), r, g, b, 255 );
        SDL_RenderFillRect( renderer_.get(), &screen_ );
    };

    void Graphics::screenBorders()
    {
        if ( WINDOW_TYPE == SDL_WINDOW_FULLSCREEN_DESKTOP )
        {
            SDL_SetRenderDrawColor( renderer_.get(), 0, 0, 0, 255 );
            SDL_RenderFillRect( renderer_.get(), &top_bar_ );
            SDL_RenderFillRect( renderer_.get(), &bottom_bar_ );
            SDL_RenderFillRect( renderer_.get(), &left_bar_ );
            SDL_RenderFillRect( renderer_.get(), &right_bar_ );
        }
    };

    void Graphics::clearScreen()
    {
        SDL_SetRenderDrawColor( renderer_.get(), 0, 0, 0, 255 );
        SDL_RenderClear( renderer_.get() );
    };

    void Graphics::presentScreen()
    {
        SDL_RenderPresent( renderer_.get() );
    };

    void Graphics::renderObject( SpriteSheet sheet, sdl2::SDLRect source, sdl2::SDLRect dest, SDL_RendererFlip flip, double rotation )
    {
        if ( sheet < SpriteSheet::LIMIT )
        {
            if ( textures_.find( sheet ) == textures_.end() )
            {
                loadTexture( sheet );
            }

            if ( textures_.find( sheet ) != textures_.end() )
            {
                dest = sourceRelativeToScreen( dest );
                if ( SDL_RenderCopyEx( renderer_.get(), textures_.at( sheet ).get(), &source, &dest, rotation, 0, flip ) != 0 )
                    printf( "Render failure: %s\n", SDL_GetError() );
            }
        }
    };

    int Graphics::magnified( int n ) const
    {
        return n * magnification_;
    };


    void Graphics::adjustMagnification()
    {
        sdl2::SDLRect r;
        if ( SDL_GetDisplayBounds( 0, &r ) != 0 )
        {
            SDL_Log( "SDL_GetDisplayBounds failed: %s", SDL_GetError() );
            magnification_ = 1;
        }
        else
        {
            int monitor_width = r.w;
            int monitor_height = r.h;

            double screen_aspect_ratio = (double)Unit::WINDOW_WIDTH_PIXELS / (double)Unit::WINDOW_HEIGHT_PIXELS;
            double monitor_aspect_ratio = (double)monitor_width / (double)monitor_height;

            if ( monitor_aspect_ratio > screen_aspect_ratio )
            {
                magnification_ = floor( monitor_height / (double)Unit::WINDOW_HEIGHT_PIXELS );
            }
            else
            {
                magnification_ = floor( monitor_width / (double)Unit::WINDOW_WIDTH_PIXELS );
            }
            if ( FORCE_MAGNIFICATION > -1 )
            {
                magnification_ = FORCE_MAGNIFICATION;
            }

            left_edge_ = ( monitor_width - magnified( Unit::WINDOW_WIDTH_PIXELS ) ) / 2;
            top_edge_ = ( monitor_height - magnified( Unit::WINDOW_HEIGHT_PIXELS ) ) / 2;

            if ( WINDOW_TYPE == SDL_WINDOW_RESIZABLE )
            {
                left_edge_ = 0;
                top_edge_ = 0;
            }

            adjustScreen();

            top_bar_ = { 0, 0, monitor_width, top_edge_ };
            bottom_bar_ = { 0, screen_.y + screen_.h, monitor_width, monitor_height - ( screen_.y + screen_.h ) };
            left_bar_ = { 0, 0, left_edge_, monitor_height };
            right_bar_ = { screen_.x + screen_.w, 0, monitor_width - ( screen_.x + screen_.w ), monitor_height };

            //screen_surface_.reset( SDL_CreateTexture( renderer_.get(), SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, screen_.w, screen_.h ) );
            SDL_SetRenderTarget( renderer_.get(), screen_surface_.get() );
        }
    };

    void Graphics::adjustScreen()
    {
        screen_.x = left_edge_;
        screen_.y = top_edge_;
        screen_.w = magnified( Unit::WINDOW_WIDTH_PIXELS );
        screen_.h = magnified( Unit::WINDOW_HEIGHT_PIXELS );
    };

    sdl2::SDLRect Graphics::sourceRelativeToScreen( sdl2::SDLRect source ) const
    {
        return
        {
            magnified( source.x ) + left_edge_,
            source.y = magnified( source.y ) + top_edge_,
            source.w = magnified( source.w ),
            source.h = magnified( source.h ),
        };
    };

    void Graphics::renderRect( sdl2::SDLRect box, int color, int alpha )
    {
        int r = 0;
        int g = 0;
        int b = 0;

        if ( palette_ != nullptr )
        {
            r = palette_->getColor( color ).r;
            g = palette_->getColor( color ).g;
            b = palette_->getColor( color ).b;
        }

        sdl2::SDLRect box_relative = sourceRelativeToScreen( box );
        SDL_SetRenderDrawColor( renderer_.get(), r, g, b, alpha );
        SDL_RenderFillRect( renderer_.get(), &box_relative );
    };

    void Graphics::newPalette( Palette::PaletteSet palette )
    {
        palette_.reset( new Palette( palette ) );
        clearTextures();
    };

    void Graphics::update() { animation_frame_.update(); };
    bool Graphics::nextFrame() { return animation_frame_.hit(); };

    int Graphics::fpsMilliseconds() const
    {
        return floor( 1000 / FPS );
    };

    const std::string Graphics::setImgPath( Game& game )
    {
        return game.resourcePath() + IMG_RELATIVE_DIR + game.pathDivider();
    };
