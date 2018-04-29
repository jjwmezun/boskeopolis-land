#include "audio.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "input.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "mezun_math.hpp"
#include "render.hpp"
#include <SDL2/SDL.h>
#include "title_state.hpp"
#include <vector>

namespace Main
{
	// Private Variables
	enum class TransitionState
	{
		__NULL,
		FADE_OUT,
		FADE_IN
	};

	enum class StateChangeType
	{
		__NULL,
		CHANGE,
		PUSH,
		POP,
		QUIT
	};

	static constexpr char TITLE[] = "Boskeopolis Land";
	static constexpr int TRANSITION_SPEED = 16;
	static constexpr int TRANSITION_LIMIT = 255;

	StateChangeType state_change_type_ = StateChangeType::__NULL;

	int ticks_ = 0;
	int graphics_ticks_ = 0;
	int frames_ = 0;
	std::string resource_path_ = "";
	std::string path_divider_ = "/";
	bool SAVING_ALLOWED = true;

	std::unique_ptr<GameState> new_state_ = {};
	std::vector< std::unique_ptr<GameState> > states_ = {}; // Polymorphism pointers.

	TransitionState transition_state_ = TransitionState::__NULL;
	int transition_level_ = 0;


	// Private Function Declarations
	void init( const std::vector<std::string>& args );
	void execute();
	void end();
	int fpsMilliseconds();
	void initSDL();
	void setResourcePath();
	void render();
	void firstState();

	void changeStateSafe();
	void pushStateSafe();
	void popStateSafe();

	void stateReset();
	bool running();
	bool testTotalQuit();
	bool canChange();


	// Function Implementations
	void stateReset()
	{	
		Input::reset();
		state_change_type_ = StateChangeType::__NULL;
	};

	bool testTotalQuit()
	{
		return state_change_type_ == StateChangeType::QUIT && transition_level_ == TRANSITION_LIMIT;
	};

	void init( const std::vector<std::string>& args )
	{
		initSDL();

		if ( args.size() >= 4 )
		{
			if ( mezun::areStringsEqual( args.at( 3 ), "no-save" ) )
			{
				SAVING_ALLOWED = false;
			}
		}
		srand ( time( nullptr ) );
		setResourcePath();
		Input::init();
		Render::init( args );
		Audio::init( args );
		firstState();
	};

	void end()
	{
		Audio::close();
		Render::quit();
		Input::close();
		SDL_Quit();
	};

	bool running()
	{
		return state_change_type_ != StateChangeType::QUIT;
	};

	void execute()
	{
		SDL_Event event;

		while ( !testTotalQuit() )
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
						//Input::axis( event.jaxis );
					break;
					case ( SDL_JOYBUTTONDOWN ):
						//Input::buttonPress( event.jbutton.button );
						//Input::buttonHold( event.jbutton.button );
					break;
					case ( SDL_JOYBUTTONUP ):
						//Input::buttonRelease( event.jbutton.button );
					break;
				}
			}

			if ( ( int )( SDL_GetTicks() ) - ticks_ >= fpsMilliseconds() )
			{
				if ( Input::pressed( Input::Action::ESCAPE ) )
				{
					quit();
				}

				switch ( state_change_type_ )
				{
					case ( StateChangeType::CHANGE ):
						changeStateSafe();
					break;
					case ( StateChangeType::POP ):
						popStateSafe();
					break;
					case ( StateChangeType::PUSH ):
						pushStateSafe();
					break;
				}

				switch ( transition_state_ )
				{
					case ( TransitionState::__NULL ):
						states_.back()->update();
					break;

					case ( TransitionState::FADE_OUT ):
						if ( transition_level_ < TRANSITION_LIMIT )
						{
							transition_level_ += TRANSITION_SPEED;
							if ( transition_level_ > TRANSITION_LIMIT )
							{
								transition_level_ = TRANSITION_LIMIT;
							}
						}
						else
						{
							transition_state_ = TransitionState::FADE_IN;
						}
					break;

					case ( TransitionState::FADE_IN ):
						states_.back()->update();
						if ( transition_level_ > 0 )
						{
							transition_level_ -= TRANSITION_SPEED;
							if ( transition_level_ < 0 )
							{
								transition_level_ = 0;
							}
						}
						else
						{
							transition_state_ = TransitionState::__NULL;
						}
					break;
				}

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

		for ( auto& st : states_ )
		{
			st->render();
		}

		switch ( transition_state_ )
		{
			case ( TransitionState::FADE_IN ):
			case ( TransitionState::FADE_OUT ):
				Render::stateChangeFade( ( Uint8 )( transition_level_ ) );
			break;
		}

		Render::screenBorders();
		Render::presentScreen();
	};

	void quit()
	{
		state_change_type_ = StateChangeType::QUIT;
		transition_state_ = TransitionState::FADE_OUT;
	};

	void initSDL()
	{
		if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
		{
			SDL_Log( "SDL_Initialization failed: %s", SDL_GetError() );
		}
	};

	void changeState( std::unique_ptr<GameState> state )
	{
		if ( new_state_.get() != nullptr ) { return; }
		new_state_.reset( state.release() );
		state_change_type_ = StateChangeType::CHANGE;
		transition_state_ = TransitionState::FADE_OUT;
	};

	bool canChange()
	{
		return transition_state_ == TransitionState::__NULL || ( transition_state_ == TransitionState::FADE_IN && transition_level_ == TRANSITION_LIMIT );
	};

	void changeStateSafe()
	{
		if ( canChange() )
		{
			assert( new_state_ );

			states_.clear();
			pushStateSafe();

			// This is a mo' important state change from the others, so we should clear out surfaces.
			Render::clearSurfaces();
		}
	};

	void pushState( std::unique_ptr<GameState> state, bool transition )
	{
		if ( new_state_.get() != nullptr ) { return; }
		new_state_.reset( state.release() );
		state_change_type_ = StateChangeType::PUSH;
		if ( transition )
		{
			transition_state_ = TransitionState::FADE_OUT;
		}
	};

	void pushStateSafe()
	{
		if ( canChange() )
		{
			assert( new_state_ );
			states_.push_back( move( new_state_ ) );
			stateReset();
			states_.back()->changePalette();
			states_.back()->init();

			// Fixes graphical bug wherein start o' level state shows before message appears due to delay
			// 'tween creating message state in init just before now & the next time this loop checks for
			// state change calls.
			if ( state_change_type_ == StateChangeType::PUSH )
			{
				pushStateSafe();
			}
		}
	};

	void popState( bool transition )
	{
		state_change_type_ = StateChangeType::POP;
		if ( transition )
		{
			transition_state_ = TransitionState::FADE_OUT;
		}
	};

	void popStateSafe()
	{
		if ( canChange() )
		{
			assert ( !states_.empty() );
			states_.pop_back();
			assert ( !states_.empty() );
			states_.back()->changePalette();
			states_.back()->backFromPop();
			stateReset();
		}
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
		return mezun::nextFrame( frames_, interval, duration );
	};

	int frame()
	{
		return frames_;
	};

	bool nextStateFrame( int interval, int duration )
	{
		return states_.back()->nextFrame( interval, duration );
	};

	int stateFrame()
	{
		return states_.back()->frame();
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