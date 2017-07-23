#include "input.hpp"
#include <map>

namespace Input
{
	// Private Variables
	std::map<Action, SDL_Keycode> buttons_ =
	{
		{ Action::CONFIRM     , SDLK_z      },
		{ Action::CANCEL      , SDLK_x      },
		{ Action::MENU        , SDLK_c      },
		{ Action::JUMP        , SDLK_z      },
		{ Action::RUN         , SDLK_x      },
		{ Action::MOVE_UP     , SDLK_UP     },
		{ Action::MOVE_RIGHT  , SDLK_RIGHT  },
		{ Action::MOVE_DOWN   , SDLK_DOWN   },
		{ Action::MOVE_LEFT   , SDLK_LEFT   },
		{ Action::CAMERA_LEFT , SDLK_a      },
		{ Action::CAMERA_RIGHT, SDLK_d      },
		{ Action::CAMERA_UP   , SDLK_w      },
		{ Action::CAMERA_DOWN , SDLK_s      },
		{ Action::ESCAPE      , SDLK_ESCAPE }
	};
	std::map<SDL_Keycode, bool> keys_pressed_;
	std::map<SDL_Keycode, bool> keys_pressed_before_released_;
	std::map<SDL_Keycode, bool> keys_released_;
	std::map<SDL_Keycode, bool> keys_held_;


	// Private Function Declarations
	void resetList( std::map<SDL_Keycode, bool>& list );


	// Function Implementations
	void reset()
	{
		resetList( keys_pressed_ );
		resetList( keys_released_ );
		resetList( keys_held_ );
	};

	void update()
	{
		resetList( keys_pressed_ );
		resetList( keys_released_ );
	};

	void resetList( std::map<SDL_Keycode, bool>& list )
	{
		for ( int i = 0; i < NUM_O_ACTIONS; ++i )
		{
			list[ buttons_.at( ( Action )i ) ] = false;
		}
	};

	bool pressed( Action action )
	{
		return keys_pressed_.at( ( buttons_.at( action ) ) );
	};

	bool pressedMain()
	{
		return pressed( Action::CONFIRM ) || pressed( Action::CANCEL ) || pressed( Action::MENU );
	};

	bool released( Action action )
	{
		return keys_released_.at( buttons_.at( action ) );
	};

	bool held( Action action )
	{
		return keys_held_.at( buttons_.at( action ) );
	};

	void keyPress( SDL_Keycode key )
	{
		if ( !keys_pressed_before_released_[ key ] )
		{
			keys_pressed_[ key ] = true;
			keys_pressed_before_released_[ key ] = true;
		}
	};

	void keyRelease( SDL_Keycode key )
	{
		keys_released_[ key ] = true;
		keys_pressed_[ key ] = false;
		keys_held_[ key ] = false;
		keys_pressed_before_released_[ key ] = false;
	};

	void keyHold( SDL_Keycode key )
	{
		keys_held_[ key ] = true;
	};
};