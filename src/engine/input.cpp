#include <cassert>
#include "input.hpp"
#include <iostream>
#include <vector>

namespace Input
{
	//
	//  PRIVATE VARIABLES
	//
	//////////////////////////////////////////////////////////

		static constexpr int DEFAULT_KEYCODE_CHANGE_VALUE = -1;

		static constexpr Uint8  NULL_BUTTON   = 255;
		static constexpr Uint8  AXIS_X_POSITIVE_BUTTON = 254;
		static constexpr Uint8  AXIS_X_NEGATIVE_BUTTON = 253;
		static constexpr Uint8  AXIS_Y_POSITIVE_BUTTON = 252;
		static constexpr Uint8  AXIS_Y_NEGATIVE_BUTTON = 251;

		static constexpr Uint8  AXIS_X        = 0;
		static constexpr Uint8  AXIS_Y        = 1;
		static constexpr Sint16 AXIS_POSITIVE = 32767;
		static constexpr Sint16 AXIS_NEGATIVE = -32768;

		static const std::string action_names_[ NUM_O_ACTIONS ] =
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
		static SDL_Keycode key_map_ [ NUM_O_ACTIONS ] =
		{
			/* CONFIRM      */ SDLK_z      ,
			/* CANCEL       */ SDLK_x      ,
			/* MENU         */ SDLK_c      ,
			/* JUMP         */ SDLK_z      ,
			/* RUN          */ SDLK_x      ,
			/* MOVE_UP      */ SDLK_UP     ,
			/* MOVE_RIGHT   */ SDLK_RIGHT  ,
			/* MOVE_DOWN    */ SDLK_DOWN   ,
			/* MOVE_LEFT    */ SDLK_LEFT   ,
			/* CAMERA_LEFT  */ SDLK_a      ,
			/* CAMERA_RIGHT */ SDLK_d      ,
			/* CAMERA_UP    */ SDLK_w      ,
			/* CAMERA_DOWN  */ SDLK_s      ,
			/* ESCAPE       */ SDLK_ESCAPE
		};

		static std::vector<SDL_Joystick*> joysticks_;

		static bool actions_pressed_ [ NUM_O_ACTIONS ];
		static bool actions_pressed_before_released_ [ NUM_O_ACTIONS ];
		static bool actions_released_ [ NUM_O_ACTIONS ];
		static bool actions_held_ [ NUM_O_ACTIONS ];

		static Uint8 controller_button_map_ [ NUM_O_ACTIONS ] =
		{
			/* CONFIRM      */ 0      ,
			/* CANCEL       */ 1      ,
			/* MENU         */ 2      ,
			/* JUMP         */ 3      ,
			/* RUN          */ 4      ,
			/* MOVE_UP      */ AXIS_Y_NEGATIVE_BUTTON,
			/* MOVE_RIGHT   */ AXIS_X_POSITIVE_BUTTON,
			/* MOVE_DOWN    */ AXIS_Y_POSITIVE_BUTTON,
			/* MOVE_LEFT    */ AXIS_X_NEGATIVE_BUTTON,
			/* CAMERA_LEFT  */ 9      ,
			/* CAMERA_RIGHT */ 10      ,
			/* CAMERA_UP    */ 11      ,
			/* CAMERA_DOWN  */ 12      ,
			/* ESCAPE       */ NULL_BUTTON
		};

		static int keycode_change_ = DEFAULT_KEYCODE_CHANGE_VALUE;
		static bool waiting_for_press_ = false;


	//
	//  PRIVATE FUNCTION DECLARATIONS
	//
	//////////////////////////////////////////////////////////

		void resetList( bool* list );
		void registerKeyPress( Action action );
		void registerKeyRelease( Action action );
		void registerKeyHold( Action action );
		void registerAxisPress( Uint8 axis, Sint16 value );
		void registerSingleAxisPress( Sint16 value, Uint8 negative, Uint8 positive );
		void registerAxisChange( Uint8 axis, Sint16 value );
		bool movingCharacterFunction( bool ( *f )( Action a ) );
		void setKeycodeChangeFinish( SDL_Keycode key );
		void setButtonChangeFinish( Uint8 button );
		void setPreferedButtonConfig();


	//
	//  FUNCTION IMPLEMENTATIONS
	//
	//////////////////////////////////////////////////////////

		template<typename T>
		void eachKey( T key, const T* list, void ( *f )( Action ) )
		{
			for ( int i = 0; i < NUM_O_ACTIONS; ++i )
			{
				if ( list[ i ] == key )
				{
					f( ( Action )( i ) );
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
			key_map_[ keycode_change_ ] = key;
			keycode_change_ = DEFAULT_KEYCODE_CHANGE_VALUE;
			waiting_for_press_ = false;
		};

		void setButtonChangeFinish( Uint8 button )
		{
			assert( keycode_change_ >= 0 && keycode_change_ < NUM_O_ACTIONS );
			controller_button_map_[ keycode_change_ ] = button;
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
			SDL_Keycode k = key_map_[ ( int )( action ) ];
			return SDL_GetKeyName( k );
		};

		std::string getButtonName( Action action )
		{
			Uint8 button = controller_button_map_[ ( int )( action ) ];

			switch ( button )
			{
				case ( AXIS_X_NEGATIVE_BUTTON ):
				{
					return "AXIS X-";
				}
				break;
				case ( AXIS_X_POSITIVE_BUTTON ):
				{
					return "AXIS X+";
				}
				break;
				case ( AXIS_Y_NEGATIVE_BUTTON ):
				{
					return "AXIS Y-";
				}
				break;
				case ( AXIS_Y_POSITIVE_BUTTON ):
				{
					return "AXIS Y+";
				}
				break;
				case ( NULL_BUTTON ):
				{
					return "--";
				}
				break;
				default:
				{
					return std::to_string( button );
				}
				break;
			}
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
				if ( key == key_map_[ ( int )( Action::CONFIRM ) ] )
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
				if ( button == controller_button_map_[ ( int )( Action::CONFIRM ) ] )
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

		void axis( const SDL_JoyAxisEvent& axis_event )
		{
			#ifdef USE_CONTROLLER
				const Uint8  axis  = axis_event.axis;
				const Sint16 value = axis_event.value;

				if ( keycode_change_ != DEFAULT_KEYCODE_CHANGE_VALUE )
				{
					registerAxisChange( axis, value );
				}
				else
				{
					registerAxisPress( axis, value );
				}
			#endif
		};

		void registerAxisPress( Uint8 axis, Sint16 value )
		{
			#ifdef USE_CONTROLLER
				switch ( axis )
				{
					case ( AXIS_X ):
						registerSingleAxisPress( value, AXIS_X_NEGATIVE_BUTTON, AXIS_X_POSITIVE_BUTTON );
					break;

					case ( AXIS_Y ):
						registerSingleAxisPress( value, AXIS_Y_NEGATIVE_BUTTON, AXIS_Y_POSITIVE_BUTTON );
					break;
				}
			#endif
		}

		void registerSingleAxisPress( Sint16 value, Uint8 negative, Uint8 positive )
		{
			#ifdef USE_CONTROLLER
				switch ( value )
				{
					case ( AXIS_NEGATIVE ):
						eachKey( negative, controller_button_map_, registerKeyPress );
						eachKey( negative, controller_button_map_, registerKeyHold );
						eachKey( positive, controller_button_map_, registerKeyRelease );
					break;

					case ( AXIS_POSITIVE ):

					eachKey( positive, controller_button_map_, registerKeyPress );
					eachKey( positive, controller_button_map_, registerKeyHold );
					eachKey( negative, controller_button_map_, registerKeyRelease );
					break;

					case ( 0 ):
						eachKey( positive, controller_button_map_, registerKeyRelease );
						eachKey( negative, controller_button_map_, registerKeyRelease );
					break;
				}
			#endif
		};

		void registerAxisChange( Uint8 axis, Sint16 value )
		{
			#ifdef USE_CONTROLLER
				switch ( axis )
				{
					case ( AXIS_X ):
					{
						switch ( value )
						{
							case ( AXIS_NEGATIVE ):
							{
								setButtonChangeFinish( AXIS_X_NEGATIVE_BUTTON );
							}
							break;
							case ( AXIS_POSITIVE ):
							{
								setButtonChangeFinish( AXIS_X_POSITIVE_BUTTON );
							}
							break;
						}
					}
					break;

					case ( AXIS_Y ):
					{
						switch ( value )
						{
							case ( AXIS_NEGATIVE ):
							{
								setButtonChangeFinish( AXIS_Y_NEGATIVE_BUTTON );
							}
							break;
							case ( AXIS_POSITIVE ):
							{
								setButtonChangeFinish( AXIS_Y_POSITIVE_BUTTON );
							}
							break;
						}
					}
					break;
				}
			#endif
		}
};
