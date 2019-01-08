#include "audio.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "console_arguments.hpp"
#include <deque>
#include "input.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "mezun_math.hpp"
#include "render.hpp"
#include <SDL2/SDL.h>
#include "title_state.hpp"
#include <vector>
#include <iostream>

namespace Main
{
	// Private Variables
	enum class StateChangeType
	{
		__NULL,
		CHANGE,
		PUSH,
		POP,
		QUIT
	};

	#ifdef USE_CONTROLLER
		static constexpr Uint32 SDL_INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK;
	#else
		static constexpr Uint32 SDL_INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	#endif

	static constexpr char TITLE[] = "Boskeopolis Land";
	static constexpr int TRANSITION_SPEED = 16;
	static constexpr int TRANSITION_LIMIT = 255;

	StateChangeType state_change_type_ = StateChangeType::__NULL;

	int ticks_ = 0;
	int graphics_ticks_ = 0;
	int frames_ = 0;
	std::string resource_path_ = "";
	std::string path_divider_ = "/";
	bool NOSAVE = false;
	bool NOHARM = false;

	std::deque< std::unique_ptr<GameState> > push_states_ = {};
	std::vector< std::unique_ptr<GameState> > states_ = {}; // Polymorphism pointers.

	TransitionState transition_state_ = TransitionState::__NULL;
	int transition_level_ = 0;


	// Private Function Declarations
	void init( int argc, char** argv );
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

	void renderStates();
	void renderTransition();


	// Function Implementations
	TransitionState transitionState()
	{
		return transition_state_;
	};

	void startFadeOut()
	{
		transition_state_ = TransitionState::FADE_OUT;
	};

	bool stillFading()
	{
		return transition_state_ == TransitionState::__NULL;
	};

	void stateReset()
	{
		Input::reset();
		state_change_type_ = StateChangeType::__NULL;
	};

	bool testTotalQuit()
	{
		return state_change_type_ == StateChangeType::QUIT && transition_level_ == TRANSITION_LIMIT;
	};

	void init( int argc, char** argv )
	{
		initSDL();
		const ConsoleArguments args( argc, argv );
		NOSAVE = args.nosave();
		NOHARM = args.noharm();
		mezun::initRand();
		setResourcePath();
		Render::init( args.windowed(), args.magnification() );
		Audio::init( args.noaudio() );
		firstState();
		Input::init();
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

			if ( ( int )( SDL_GetTicks() ) - ticks_ >= fpsMilliseconds() )
			{
				if ( Input::exitButtonHeldLongEnough() )
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
		renderStates();
		Input::renderQuitText();
		renderTransition();
		Render::screenBorders();
		Render::presentScreen();
	};

	void renderStates()
	{
		int i = states_.size() - 1;
		while ( i > 0 )
		{
			if ( !states_[ i ]->testRenderBelow() ) { break; }
			--i;
		}

		while ( i < states_.size() )
		{
			states_[ i ]->render();
			++i;
		}
	};

	void renderTransition()
	{
		switch ( transition_state_ )
		{
			case ( TransitionState::FADE_IN ):
			case ( TransitionState::FADE_OUT ):
				Render::stateChangeFade( ( Uint8 )( transition_level_ ) );
			break;
		}
	};

	void quit()
	{
		state_change_type_ = StateChangeType::QUIT;
		transition_state_ = TransitionState::FADE_OUT;
	};

	void initSDL()
	{
		if ( SDL_Init( SDL_INIT_FLAGS ) != 0 )
		{
			SDL_Log( "SDL_Initialization failed: %s", SDL_GetError() );
		}
	};

	void changeState( std::unique_ptr<GameState> state )
	{
		push_states_.emplace_back( std::unique_ptr<GameState> ( state.release() ) );
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
			states_.clear();
			pushStateSafe();

			// This is a mo' important state change from the others, so we should clear out surfaces.
			Render::clearSurfaces();
		}
	};

	void pushState( std::unique_ptr<GameState> state, bool transition )
	{
		push_states_.emplace_back( std::unique_ptr<GameState> ( state.release() ) );
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
			if ( !push_states_.empty() )
			{
				states_.push_back( move( push_states_[ 0 ] ) );
				stateReset();
				states_.back()->changePalette();
				states_.back()->init();
				push_states_.pop_front();
				pushStateSafe();
			}

			// Fixes graphical bug wherein start o' level state shows before message appears due to delay
			// 'tween creating message state in init just before now & the next time this loop checks for
			// state change calls.
			if ( state_change_type_ == StateChangeType::PUSH )
			{
				//pushStateSafe();
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

	bool nosave()
	{
		return NOSAVE;
	};

	bool noharm()
	{
		return NOHARM;
	};

	std::string savePath()
	{
		return resourcePath() + "saves" + pathDivider() + "save.bin";
	};
};

int main( int argc, char* argv[] )
{
    Main::init( argc, argv );

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
