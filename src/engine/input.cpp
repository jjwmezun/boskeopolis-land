#include <cassert>
#include "input.hpp"
#include <iostream>
#include <vector>

namespace Input
{
	constexpr int DEFAULT_KEYCODE_CHANGE_VALUE = -1;
	constexpr Uint8  AXIS_X        = 0;
	constexpr Uint8  AXIS_Y        = 1;
	constexpr Sint16 AXIS_POSITIVE = 32767;
	constexpr Sint16 AXIS_NEGATIVE = -32768;

	const std::string action_names_[ NUM_O_ACTIONS ] =
	{
		"Confirm",
		"Cancel",
		"Menu",
		"Jump",
		"Run",
		"Up",
		"Right",
		"Down",
		"Left",
		"Camera Left",
		"Camera Right",
		"Camera Up",
		"Camera Down",
		"Quit"
	};

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

	Action action_x_negative_ = Action::MOVE_LEFT;
	Action action_x_positive_ = Action::MOVE_RIGHT;
	Action action_y_negative_ = Action::MOVE_UP;
	Action action_y_positive_ = Action::MOVE_DOWN;
	std::vector<SDL_Joystick*> joysticks_;

	bool actions_pressed_ [ NUM_O_ACTIONS ];
	bool actions_pressed_before_released_ [ NUM_O_ACTIONS ];
	bool actions_released_ [ NUM_O_ACTIONS ];
	bool actions_held_ [ NUM_O_ACTIONS ];

	std::vector<Uint8> controller_button_map_ [ NUM_O_ACTIONS ] =
	{
		/* CONFIRM      */ { 0 }      ,
		/* CANCEL       */ { 1 }      ,
		/* MENU         */ { 2 }      ,
		/* JUMP         */ { 3 }      ,
		/* RUN          */ { 4 }      ,
		/* MOVE_UP      */ { 5 }     ,
		/* MOVE_RIGHT   */ { 6 }  ,
		/* MOVE_DOWN    */ { 7 }   ,
		/* MOVE_LEFT    */ { 8 }   ,
		/* CAMERA_LEFT  */ { 9 }      ,
		/* CAMERA_RIGHT */ { 10 }      ,
		/* CAMERA_UP    */ { 11 }      ,
		/* CAMERA_DOWN  */ { 12 }      ,
		/* ESCAPE       */ { 13 }
	};

	int keycode_change_ = DEFAULT_KEYCODE_CHANGE_VALUE;
	bool waiting_for_press_ = false;


	// Private Function Declarations
	void resetList( bool* list );
	void registerKeyPress( Action action );
	void registerKeyRelease( Action action );
	void registerKeyHold( Action action );
	void registerAxis( Sint16 value, Action negative, Action positive );
	bool movingCharacterFunction( bool ( *f )( Action a ) );
	void setKeycodeChangeFinish( SDL_Keycode key );
	void setButtonChangeFinish( Uint8 button );


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
		#ifdef USE_CONTROLLER
			for ( int i = 0; i < SDL_NumJoysticks(); ++i )
			{
				joysticks_.push_back( SDL_JoystickOpen( i ) );
			}
		#endif
		reset();
	};

	void close()
	{
		#ifdef USE_CONTROLLER
			for ( auto j : joysticks_ )
			{
				if ( j != nullptr )
				{
					SDL_JoystickClose( j );
				}
			}
			SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
		#endif
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

	void setKeycodeChangeStart( Action action )
	{
		keycode_change_ = ( int )( action );
	};

	void setKeycodeChangeFinish( SDL_Keycode key )
	{
		assert( keycode_change_ >= 0 && keycode_change_ < NUM_O_ACTIONS );
		key_map_[ keycode_change_ ][ 0 ] = key;
		keycode_change_ = DEFAULT_KEYCODE_CHANGE_VALUE;
		waiting_for_press_ = false;
	};

	void setButtonChangeFinish( Uint8 button )
	{
		assert( keycode_change_ >= 0 && keycode_change_ < NUM_O_ACTIONS );
		controller_button_map_[ keycode_change_ ][ 0 ] = button;
		keycode_change_ = DEFAULT_KEYCODE_CHANGE_VALUE;
		waiting_for_press_ = false;
	};

	bool testKeycodeChangeDone()
	{
		return keycode_change_ == DEFAULT_KEYCODE_CHANGE_VALUE;
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

	bool movingCharacter()
	{
		movingCharacterFunction( &held );
	};

	bool movingCharacterNoHold()
	{
		movingCharacterFunction( &pressed );
	};

	bool movingCharacterFunction( bool ( *f )( Action a ) )
	{
		return
		(
			f( Action::JUMP )      ||
			f( Action::MOVE_DOWN ) ||
			f( Action::MOVE_LEFT ) ||
			f( Action::MOVE_RIGHT )
		);
	};

	std::string getActionName( Action action )
	{
		return action_names_[ ( int )( action ) ];
	};

	int calculateMaxActionNameLength()
	{
		int length = 0;
		for ( int i = 0; i < NUM_O_ACTIONS; ++i )
		{
			if ( length < action_names_[ i ].size() )
			{
				length = action_names_[ i ].size();
			}
		}
		return length;
	};

	std::string getKeyName( Action action )
	{
		SDL_Keycode k = key_map_[ ( int )( action ) ][ 0 ];
		return SDL_GetKeyName( k );
	};

	std::string getButtonName( Action action )
	{
		Uint8 button = controller_button_map_[ ( int )( action ) ][ 0 ];
		return std::to_string( button );
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
		if ( keycode_change_ != DEFAULT_KEYCODE_CHANGE_VALUE )
		{
			if ( waiting_for_press_ )
			{
				setKeycodeChangeFinish( key );
			}
		}
		else
		{
			eachKey( key, key_map_, registerKeyPress );
		}
	};

	void keyRelease( SDL_Keycode key )
	{
		if ( keycode_change_ != DEFAULT_KEYCODE_CHANGE_VALUE )
		{
			if ( key == key_map_[ ( int )( Action::CONFIRM ) ][ 0 ] )
			{
				waiting_for_press_ = true;
			}
		}
		else
		{
			eachKey( key, key_map_, registerKeyRelease );
		}
	};

	void keyHold( SDL_Keycode key )
	{
		eachKey( key, key_map_, registerKeyHold );
	};

	void buttonPress( Uint8 button )
	{
		eachKey( button, controller_button_map_, registerKeyPress );
	};

	void buttonRelease( Uint8 button )
	{
		if ( keycode_change_ != DEFAULT_KEYCODE_CHANGE_VALUE )
		{
			if ( button == controller_button_map_[ ( int )( Action::CONFIRM ) ][ 0 ] )
			{
				waiting_for_press_ = true;
			}
		}
		else
		{
			eachKey( button, controller_button_map_, registerKeyRelease );
		}
	};

	void buttonHold( Uint8 button )
	{
		if ( keycode_change_ != DEFAULT_KEYCODE_CHANGE_VALUE )
		{
			if ( waiting_for_press_ )
			{
				setButtonChangeFinish( button );
			}
		}
		else
		{
			eachKey( button, controller_button_map_, registerKeyHold );
		}
	};

	void registerAxis( Sint16 value, Action negative, Action positive )
	{
		#ifdef USE_CONTROLLER
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
		#endif
	};

	void axis( const SDL_JoyAxisEvent& axis_event )
	{
		#ifdef USE_CONTROLLER
			switch ( axis_event.axis )
			{
				case ( AXIS_X ):
					registerAxis( axis_event.value, action_x_negative_, action_x_positive_ );
				break;

				case ( AXIS_Y ):
					registerAxis( axis_event.value, action_y_negative_, action_y_positive_ );
				break;
			}
		#endif
	};
};
