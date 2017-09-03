#include "input.hpp"
#include <iostream>
#include <vector>

namespace Input
{
	//constexpr Uint8  AXIS_X        = 0;
	//constexpr Uint8  AXIS_Y        = 1;
	//constexpr Sint16 AXIS_POSITIVE = 32767;
	//constexpr Sint16 AXIS_NEGATIVE = -32768;

	// Private Variables
	std::vector<SDL_Keycode> key_map_ [ NUM_O_ACTIONS ] =
	{
		/* CONFIRM      */ { SDLK_z }      ,
		/* CANCEL       */ { SDLK_x }      ,
		/* MENU         */ { SDLK_c }      ,
		/* JUMP         */ { SDLK_z }      ,
		/* RUN          */ { SDLK_x }      ,
		/* MOVE_UP      */ { SDLK_UP }     ,
		/* MOVE_RIGHT   */ { SDLK_RIGHT }  ,
		/* MOVE_DOWN    */ { SDLK_DOWN }   ,
		/* MOVE_LEFT    */ { SDLK_LEFT }   ,
		/* CAMERA_LEFT  */ { SDLK_a }      ,
		/* CAMERA_RIGHT */ { SDLK_d }      ,
		/* CAMERA_UP    */ { SDLK_w }      ,
		/* CAMERA_DOWN  */ { SDLK_s }      ,
		/* ESCAPE       */ { SDLK_ESCAPE }
	};

	//Action action_x_negative_ = Action::MOVE_LEFT;
	//Action action_x_positive_ = Action::MOVE_RIGHT;
	//Action action_y_negative_ = Action::MOVE_UP;
	//Action action_y_positive_ = Action::MOVE_DOWN;
	
	//std::vector<SDL_Joystick*> joysticks_;

	bool actions_pressed_ [ NUM_O_ACTIONS ];
	bool actions_pressed_before_released_ [ NUM_O_ACTIONS ];
	bool actions_released_ [ NUM_O_ACTIONS ];
	bool actions_held_ [ NUM_O_ACTIONS ];


	// Private Function Declarations
	void resetList( bool* list );
	void registerKeyPress( Action action );
	void registerKeyRelease( Action action );
	void registerKeyHold( Action action );
	void registerAxis( Sint16 value, Action negative, Action positive );


	// Function Implementations	
	template<typename T>
	void eachKey( T key, const std::vector<T>* list, void ( *f )( Action ) )
	{
		for ( int i = 0; i < NUM_O_ACTIONS; ++i )
		{
			for ( auto j : list[ i ] )
			{
				if ( j == key )
				{
					f( ( Action )( i ) );
				}
			}
		}
	};

	void init()
	{
		/*SDL_InitSubSystem( SDL_INIT_JOYSTICK );
		for ( int i = 0; i < SDL_NumJoysticks(); ++i )
		{
			joysticks_.push_back( SDL_JoystickOpen( i ) );
		}*/
		reset();
	};
	
	void close()
	{/*
		for ( auto j : joysticks_ )
		{
			if ( j != nullptr )
			{
				SDL_JoystickClose( j );
			}
		}
		SDL_QuitSubSystem( SDL_INIT_JOYSTICK );*/
	};

	void reset()
	{
		resetList( actions_pressed_ );
		resetList( actions_released_ );
		resetList( actions_held_ );
	};

	void update()
	{	
		resetList( actions_pressed_ );
		resetList( actions_released_ );
	};

	void resetList( bool* list )
	{
		for ( int i = 0; i < NUM_O_ACTIONS; ++i )
		{
			list[ i ] = false;
		}
	};

	bool pressed( Action action )
	{
		return actions_pressed_[ ( int )( action ) ];
	};

	bool pressedMain()
	{
		return pressed( Action::CONFIRM ) || pressed( Action::CANCEL ) || pressed( Action::MENU );
	};

	bool released( Action action )
	{
		return actions_released_[ ( int )( action ) ];
	};

	bool held( Action action )
	{
		return actions_held_[ ( int )( action ) ];
	};

	void registerKeyPress( Action action )
	{
		if ( !actions_pressed_before_released_[ ( int )( action ) ] )
		{
			actions_pressed_[ ( int )( action ) ] = true;
			actions_pressed_before_released_[ ( int )( action ) ] = true;
		}
	};

	void registerKeyRelease( Action action )
	{
		actions_released_[ ( int )( action ) ] = true;
		actions_pressed_[ ( int )( action ) ] = false;
		actions_held_[ ( int )( action ) ] = false;
		actions_pressed_before_released_[ ( int )( action ) ] = false;
	};

	void registerKeyHold( Action action )
	{
		actions_held_[ ( int )( action ) ] = true;
	};
	
	void keyPress( SDL_Keycode key )
	{
		eachKey( key, key_map_, registerKeyPress );
	};

	void keyRelease( SDL_Keycode key )
	{
		eachKey( key, key_map_, registerKeyRelease );
	};

	void keyHold( SDL_Keycode key )
	{
		eachKey( key, key_map_, registerKeyHold );
	};
	/*
	void buttonPress( Uint8 button )
	{
		eachKey( button, controller_button_map_, registerKeyPress );
	};

	void buttonRelease( Uint8 button )
	{
		eachKey( button, controller_button_map_, registerKeyRelease );
	};

	void buttonHold( Uint8 button )
	{
		eachKey( button, controller_button_map_, registerKeyHold );
	};
	
	void registerAxis( Sint16 value, Action negative, Action positive )
	{		
		switch ( value )
		{
			case ( AXIS_NEGATIVE ):
				registerKeyPress( negative );
				registerKeyHold( negative );
				registerKeyRelease( positive );
			break;

			case ( AXIS_POSITIVE ):
				registerKeyPress( positive );
				registerKeyHold( positive );
				registerKeyRelease( negative );
			break;
				
			case ( 0 ):
				registerKeyRelease( negative );
				registerKeyRelease( positive );
			break;
		}
	};
	
	void axis( const SDL_JoyAxisEvent& axis_event )
	{
		switch ( axis_event.axis )
		{
			case ( AXIS_X ):
				registerAxis( axis_event.value, action_x_negative_, action_x_positive_ );
			break;

			case ( AXIS_Y ):
				registerAxis( axis_event.value, action_y_negative_, action_y_positive_ );
			break;
		}
	};*/
};