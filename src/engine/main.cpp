#include "audio.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "console_arguments.hpp"
#include <deque>
#include <filesystem>
#include "input.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "level.hpp"
#include "main.hpp"
#include "message_state.hpp"
#include "mezun_helpers.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_math.hpp"
#include "render.hpp"
#include <SDL2/SDL.h>
#include "title_state.hpp"
#include <vector>
#include "wmessage_state.hpp"

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

	static constexpr int TRANSITION_SPEED = 16;
	static constexpr int TRANSITION_LIMIT = 255;

	StateChangeType state_change_type_ = StateChangeType::__NULL;

	int ticks_ = 0;
	int updates_per_frame_ = 0;
	int frames_ = 0;
	std::string resource_path_ = "";
	std::string save_path_ = "";
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
	void setResourcePaths();
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

	void maintainFrameRate();


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
		//Input::reset();
		state_change_type_ = StateChangeType::__NULL;
	};

	bool testTotalQuit()
	{
		return state_change_type_ == StateChangeType::QUIT && transition_level_ == TRANSITION_LIMIT;
	};

	void init( int argc, char** argv )
	{
		initSDL();
		setResourcePaths();
		Localization::init();
		const ConsoleArguments args( argc, argv );
		NOSAVE = args.nosave();
		NOHARM = args.noharm();
		mezun::initRand();
		Render::init( args.windowed(), args.magnification() );
		Audio::init( args.noaudio() );
		try
		{
			Level::buildLevelList();
			firstState();
		}
		catch ( const mezun::CantLoadLevelNames e )
		{
			Main::changeState
			(
				std::unique_ptr<MessageState> ( MessageState::errorMessage( e.what() ) )
			);
		}
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

			maintainFrameRate();

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
	};

	void render()
	{
		Render::clearScreen();
		renderStates();
		Input::renderQuitText();
		renderTransition();
		Render::presentScreen();
	};

	void renderStates()
	{
		unsigned int i = states_.size() - 1;
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

		Audio::playSound( Audio::SoundType::SELECT );
		states_.push_back
		(
			std::unique_ptr<GameState>
			(
				new WMessageState
				(
					{ Localization::getCurrentLanguage().getIntroText() },
					WMessageState::Type::CHANGE,
					{ "Grayscale", 1 },
					std::unique_ptr<GameState> ( new TitleState() )
				)
			)
		);
		states_.back()->changePalette();
		states_.back()->init();
		transition_state_ = TransitionState::FADE_IN;
		transition_level_ = TRANSITION_LIMIT;
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

	void setResourcePaths()
	{
		#ifdef _WIN32
			path_divider_ = "\\";
		#endif

		char* base_path_c = SDL_GetBasePath();
		std::string base_path = std::string( base_path_c );
		SDL_free( base_path_c );
		resource_path_ = base_path + pathDivider() + "assets" + pathDivider();
		save_path_ = base_path + pathDivider() + "saves" + pathDivider();
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

	std::string savePath( int n )
	{
		return save_path_ + "save-" + std::to_string( n ) + ".bin";
	};

	std::string saveDirectory()
	{
		return save_path_;
	}

	bool testHaveSaves()
	{
		std::filesystem::path directory{ save_path_ };
		return !std::filesystem::is_empty( directory );
	};

	const Palette& getPalette()
	{
		return states_.back()->palette();
	}

	void maintainFrameRate()
	{
		if ( ( int )( SDL_GetTicks() ) - ticks_ < fpsMilliseconds() )
		{
			SDL_Delay( std::max( 0, fpsMilliseconds() - ( ( int )( SDL_GetTicks() ) - ticks_ ) ) );
		}
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