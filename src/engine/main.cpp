#include <cassert>
#include "input.hpp"
#include "main.hpp"
#include "message_state.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include <SDL2/SDL.h>
#include "title_state.hpp"
#include <vector>

namespace Main
{
	// Private Variables
	constexpr char TITLE[] = "Boskeopolis Land";
	bool running_ = true;
	bool state_change_ = false;
	bool state_is_new_ = false;
	bool state_pop_    = false;

	int ticks_ = 0;
	int graphics_ticks_ = 0;
	int frames_ = 0;
	std::string resource_path_ = "";
	std::string path_divider_ = "/";
	bool SAVING_ALLOWED = true;

	std::unique_ptr<GameState> new_state_ = {};
	std::vector< std::unique_ptr<GameState> > states_ = {}; // Polymorphism pointers.


	// Private Function Declarations
	bool init( const std::vector<std::string>& args );
	int fpsMilliseconds();
	void initSDL();
	void setResourcePath();
	void render();
	void firstState();

	void changeStateSafe();
	void popStateSafe();


	// Function Implementations
	bool init( const std::vector<std::string>& args )
	{
		initSDL();

		if ( args.size() >= 4 )
		{
			if ( mezun::areStringsEqual( args.at( 3 ), "no-save" ) )
			{
				SAVING_ALLOWED = false;
			}
		}

		setResourcePath();
		Render::init( args );
		Input::init();
		firstState();
	};

	void end()
	{
		Render::quit();
		Input::close();
		SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
		SDL_Quit();
	};

	bool running()
	{
		return running_;
	};

	void execute()
	{
		SDL_Event event;

		while ( running_ )
		{
			while ( SDL_PollEvent( &event ) != 0 )
			{
				switch ( event.type )
				{
					case ( SDL_QUIT ):
						quit();
					break;
					case ( SDL_KEYDOWN ):
						Input::keyPress( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
						Input::keyHold( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
					break;
					case ( SDL_KEYUP ):
						Input::keyRelease( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
					break;
					case ( SDL_JOYAXISMOTION ):
						Input::axis( event.jaxis );
					break;
					case ( SDL_JOYBUTTONDOWN ):
						Input::buttonPress( event.jbutton.button );
						Input::buttonHold( event.jbutton.button );
					break;
					case ( SDL_JOYBUTTONUP ):
						Input::buttonRelease( event.jbutton.button );
					break;
				}
			}

			if ( SDL_GetTicks() - ticks_ >= fpsMilliseconds() )
			{
				if ( Input::pressed( Input::Action::ESCAPE ) )
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
					Input::reset();
					state_change_ = false;
				}

				states_.back()->update();

				++frames_;
				ticks_ = SDL_GetTicks();

				Input::update();
				render();
			}
		}
	};

	void render()
	{
		Render::clearScreen();
		Render::colorCanvas();
		for ( auto& st : states_ )
		{
			st->render();
		}
		Render::screenBorders();
		Render::presentScreen();
	};

	void quit()
	{
		running_ = false;
	};

	void initSDL()
	{
		if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
		{
			SDL_Log( "SDL_Initialization failed: %s", SDL_GetError() );
		}
		//SDL_InitSubSystem( SDL_INIT_JOYSTICK );
	};

	void changeState( std::unique_ptr<GameState> state )
	{
		new_state_.reset( state.release() );
		state_change_ = true;
		state_is_new_ = true;
	};

	void changeStateSafe()
	{
		assert( new_state_ );

		states_.clear();
		pushState( move( new_state_ ) );
		state_is_new_ = false;

		// This is a mo' important state change from the others, so we should clear out surfaces.
		Render::clearSurfaces();
	};

	void pushState  ( std::unique_ptr<GameState> state )
	{
		states_.push_back( move(state) );
		states_.back()->changePalette();
		states_.back()->init();
		state_change_ = true;
	};

	void popState()
	{
		state_change_ = true;
		state_pop_    = true;
	};

	void popStateSafe()
	{
		assert ( !states_.empty() );

		states_.pop_back();

		assert ( !states_.empty() );

		states_.back()->changePalette();

		state_pop_ = false;
	};

	void tempMessage( std::string message, Palette&& palette, Text::FontShade text_color )
	{
		pushState( std::unique_ptr<GameState> ( new MessageState( message, true, nullptr, false, palette, text_color ) ) );
	};

	int fpsMilliseconds()
	{
		return floor( 1000 / Unit::FPS );
	};

	void firstState()
	{
		//Ensure state list is clear:
		states_.clear();

		pushState( std::unique_ptr<GameState> ( new TitleState() ) );
	};

	bool nextFrame( int interval, int duration )
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

	int frame()
	{
		return frames_;
	};

	void setResourcePath()
	{
		#ifdef _WIN32
			path_divider_ = "\\";
		#endif

		const std::string BASE_DIR = "boskeopolis-land";
		const std::string RES_SUB_DIR = pathDivider() + "resources/";
		char* base_path = SDL_GetBasePath();
		std::string path = base_path;
		SDL_free( base_path );
		size_t pos = path.find( BASE_DIR );
		path.replace( pos + BASE_DIR.length(), path.length() - pos - BASE_DIR.length(), RES_SUB_DIR );
		resource_path_ = path;
		/* WARNING: fix */
		resource_path_ = "/home/jjwmezun/Documents/cpp-projects/boskeopolis-land/resources/";
	};

	std::string resourcePath()
	{
		return resource_path_;
	};

	std::string pathDivider()
	{
		return path_divider_;
	};

	bool savingAllowed()
	{
		return SAVING_ALLOWED;
	};

	std::string savePath()
	{
		return resourcePath() + "saves" + pathDivider() + "save.bin";
	};
};

int main( int argc, char* argv[] )
{
	std::vector<std::string> args;
	for ( int i = 0; i < argc; ++i )
	{
		args.push_back( argv[ i ] );
	}

    Main::init( args );

    // If game starts without running, there was an error.
    if ( !Main::running() )
    {
		Main::end();
        return 1;
    }

    while ( Main::running() )
    {
        Main::execute();
    }

	Main::end();
    return 0;
};