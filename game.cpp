

// DEPENDENCIES
//===================================

	#include <cassert>
    #include "game.h"
    #include "title_state.h"
    #include <SDL2/SDL.h>
    #include <iostream>


// STATIC PROPERTIES
//===================================

    int Game::frames_ = 0;
    std::string Game::resource_path_ = "";
    std::string Game::path_divider_ = "/";
    bool Game::SAVING_ALLOWED = true;


// METHODS
//===================================

    Game::Game( std::vector<std::string>& args )
    :
        input_ ( Input() )
    {
        if ( !initSDL() )
            running_ = false;

        if ( args.size() >= 4 )
        {
            if ( args.at( 3 ).compare( "no-save" ) == 0 )
            {
                SAVING_ALLOWED = false;
            }
        }

        setResourcePath();
        graphics_.reset( new Graphics( *this, args ) );
        firstState();
    };

    Game::~Game()
    {
        SDL_Quit();
    };

    bool Game::running() const
    {
        return running_;
    };

    void Game::execute()
    {
        SDL_Event event;

        while ( running_ )
        {
            while ( SDL_PollEvent( &event ) != 0 )
            {
                if ( event.type == SDL_QUIT )
                {
                    quit();
                }
                if ( event.type == SDL_KEYDOWN )
                {
                    input_.keyPress( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
                    input_.keyHold( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
                }
                if ( event.type == SDL_KEYUP )
                {
                    input_.keyRelease( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
                }
            }

            if ( SDL_GetTicks() - ticks_ >= fpsMilliseconds() )
            //if ( SDL_GetTicks() % fpsMilliseconds() == 0 )
            {

                if ( input_.pressed( Input::Action::ESCAPE ) )
                {
                    running_ = false;
                }

				if ( state_is_new_ )
				{
					changeStateSafe();
				}
				else if ( state_pop_ )
				{
					popStateSafe();
				}
				
                if ( state_change_ )
                {
                    input_.reset();
                    state_change_ = false;
                }

                states_.back()->update( *this, input_, *graphics_ );
                Graphics::update();
                ticks_ = SDL_GetTicks();
                ++frames_;
                input_.update();
                render();
            }
        }
    };

    void Game::render()
    {
        graphics_->clearScreen();
        graphics_->colorCanvas();
        for ( auto &st : states_ )
        {
            st->render( *graphics_ );
        }
        graphics_->screenBorders();
        graphics_->presentScreen();
    };

    void Game::quit()
    {
        running_ = false;
    };

    bool Game::initSDL()
    {
        if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
        {
            SDL_Log( "SDL_Initialization failed: %s", SDL_GetError() );
            return false;
        }

        return true;
    };

    void Game::changeState( std::unique_ptr<GameState> state )
	{
		new_state_.reset( state.release() );
		state_change_ = true;
		state_is_new_ = true;
	};

    void Game::changeStateSafe()
    {
		assert( new_state_ );
		
        states_.clear();
        pushState( move( new_state_ ) );
        graphics_->clearSurfaces();
		state_is_new_ = false;
    };

    void Game::pushState  ( std::unique_ptr<GameState> state )
    {
        states_.push_back( move(state) );
        states_.back()->changePalette( *graphics_ );
        states_.back()->init( *this, *graphics_ );
        state_change_ = true;
    };

    void Game::popState()
	{
		state_change_ = true;
		state_pop_    = true;
	};

    void Game::popStateSafe()
    {
        assert ( !states_.empty() );
		
		states_.pop_back();
		
        assert ( !states_.empty() );
		
        states_.back()->backFromPop( *this, *graphics_ );
        states_.back()->changePalette( *graphics_ );
		
		state_pop_ = false;
    };

    int Game::fpsMilliseconds() const
    {
        return floor( 1000 / Unit::FPS );
    };

    void Game::firstState()
    {
        //Ensure state list is clear:
        states_.clear();

        pushState( std::unique_ptr<GameState> ( new TitleState() ) );
    };

    bool Game::nextFrame( int interval, int duration )
    {
        for ( int i = 0; i < duration; ++i )
        {
            if ( frames_ % interval == i )
            {
                return true;
            }
        }

        return false;
    };

    int Game::frame()
    {
        return frames_;
    };

    void Game::setResourcePath()
    {
        #ifdef _WIN32
            path_divider_ = "\\";
        #endif

        const std::string BASE_DIR = "boskeopolis-land";
        const std::string RES_SUB_DIR = pathDivider() + "resources/";
        std::string path = SDL_GetBasePath();
        size_t pos = path.find( BASE_DIR );
        path.replace( pos + BASE_DIR.length(), path.length() - pos - BASE_DIR.length(), RES_SUB_DIR );
        resource_path_ = path;
    };

    const std::string Game::resourcePath()
    {
        return resource_path_;
    };

    const std::string Game::pathDivider()
    {
        return path_divider_;
    };

    bool Game::savingAllowed()
    {
        return SAVING_ALLOWED;
    };

    const std::string Game::savePath()
    {
        return resourcePath() + "saves" + pathDivider() + "save.bin";
    };
