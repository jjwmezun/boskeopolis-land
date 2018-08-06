#include "audio.hpp"
#include <cassert>
#include <fstream>
#include "input.hpp"
#include <iostream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "mezun_json.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <vector>

namespace Input
{
	//
	//  PRIVATE VARIABLES
	//
	//////////////////////////////////////////////////////////

		static constexpr int DEFAULT_KEYCODE_CHANGE_VALUE = -1;
		static constexpr int ESCAPE_TIMER_THRESHOLD = 30;

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
			"Camera Down"
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
			/* CAMERA_DOWN  */ SDLK_s
		};

		static std::vector<SDL_Joystick*> joysticks_;

		static bool actions_pressed_ [ NUM_O_ACTIONS ];
		static bool actions_pressed_before_released_ [ NUM_O_ACTIONS ];
		static bool actions_released_ [ NUM_O_ACTIONS ];
		static bool actions_held_ [ NUM_O_ACTIONS ];

		static Uint8 controller_button_map_ [ NUM_O_ACTIONS ] =
		{
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON,
			NULL_BUTTON
		};

		static int keycode_change_ = DEFAULT_KEYCODE_CHANGE_VALUE;
		static bool waiting_for_press_ = false;
		static bool escape_held_ = false;
		static int escape_timer_ = 0;



	//
	//  PRIVATE FUNCTION DECLARATIONS
	//
	//////////////////////////////////////////////////////////

		void loadJoysticks();
		void loadConfig();
		void loadConfigFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& document_object );
		void saveConfig();
		std::string getConfigFilename();
		void resetList( bool* list );
		void updateEscape();
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
			loadJoysticks();
			loadConfig();
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

		void loadJoysticks()
		{
			#ifdef USE_CONTROLLER
				for ( int i = 0; i < SDL_NumJoysticks(); ++i )
				{
					joysticks_.push_back( SDL_JoystickOpen( i ) );
				}
			#endif
		};

		template<typename T>
		void loadConfigType( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& document_object, const char* type, T* key_map )
		{
			if ( document_object.HasMember( type ) && document_object[ type ].IsObject() )
			{
				auto keys = document_object[ type ].GetObject();
				for ( int i = 0; i < NUM_O_ACTIONS; ++i )
				{
					const char* input_name = action_names_[ i ].c_str();
					if ( keys.HasMember( input_name ) && keys[ input_name ].IsInt() )
					{
						key_map[ i ] = ( T )( keys[ input_name ].GetInt() );
					}
				}
			}
		};

		void loadConfig()
		{
			mezun::loadJSON
			(
				getConfigFilename(),
				loadConfigFunction,
				"Input configuration is missing.\nConfig file has been replaced with default.",
				"Input configuration file has been corruped.\nConfig file has been replaced with default.",
				saveConfig,
				saveConfig
			);
		};

		void loadConfigFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& document_object )
		{
			loadConfigType( document_object, "keys", key_map_ );
			loadConfigType( document_object, "buttons", controller_button_map_ );
		};

		template <typename T>
		std::string saveConfigType( const std::string& type, T* key_map, bool last = false )
		{
			std::string text = mezun::addLine( "\"" + type + "\":", 1 );
			text += mezun::addLine( "{", 1 );
			for ( int i = 0; i < NUM_O_ACTIONS; ++i )
			{
				const std::string& input_name = action_names_[ i ];
				const std::string input_value = std::to_string( ( int )( key_map[ i ] ) );
				const std::string end = ( i == NUM_O_ACTIONS - 1 ) ? "" : ",";
				text += mezun::addLine( "\"" + input_name + "\": " + input_value + end, 2 );
			}
			text += ( last ) ? mezun::addLine( "}", 1 ) : mezun::addLine( "},", 1 );
			return text;
		};

		void saveConfig()
		{
			std::ofstream ofs( getConfigFilename() );

			std::string text = "";

			text += mezun::addLine( "{", 0 );
			text += saveConfigType( "keys", key_map_ );
			text += saveConfigType( "buttons", controller_button_map_, true );
			text += mezun::addLine( "}", 0 );

			ofs << text;
			ofs.close();
		}

		std::string getConfigFilename()
		{
			return Main::resourcePath() + "config.json";
		};

		void reset()
		{
			resetList( actions_pressed_ );
			resetList( actions_released_ );
			resetList( actions_held_ );
		};

		void renderQuitText()
		{
			if ( escape_timer_ > 0 )
			{
				Text::renderText
				(
					"Quitting...",
					8,
					Unit::WINDOW_HEIGHT_PIXELS - 16,
					nullptr,
					Text::FontColor::WHITE,
					Text::DEFAULT_LINE_LENGTH,
					Text::FontAlign::LEFT,
					Text::FontColor::BLACK
				);
			}
		};

		void update()
		{
			updateEscape();
			resetList( actions_pressed_ );
			resetList( actions_released_ );
		};

		void updateEscape()
		{
			if ( escape_held_ )
			{
				++escape_timer_;
			}
			else
			{
				--escape_timer_;
				if ( escape_timer_ < 0 )
				{
					escape_timer_ = 0;
				}
			}
		};

		void setKeycodeChangeStart( Action action )
		{
			keycode_change_ = ( int )( action );
		};

		template <typename T>
		void setChangeFinish( T key, T* key_map, bool change = true )
		{
			assert( keycode_change_ >= 0 && keycode_change_ < NUM_O_ACTIONS );
			if ( change )
			{
				key_map[ keycode_change_ ] = key;
				Audio::playSound( Audio::SoundType::CONFIRM );
			}
			else
			{
				Audio::playSound( Audio::SoundType::CANCEL );
			}
			keycode_change_ = DEFAULT_KEYCODE_CHANGE_VALUE;
			waiting_for_press_ = false;
			saveConfig();
		};

		void setKeycodeChangeFinish( SDL_Keycode key )
		{
			setChangeFinish( key, key_map_, key != SDLK_ESCAPE );
		};

		void setButtonChangeFinish( Uint8 button )
		{
			setChangeFinish( button, controller_button_map_ );
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
			if ( key == SDLK_ESCAPE )
			{
				escape_held_ = false;
			}
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
			if ( key == SDLK_ESCAPE )
			{
				escape_held_ = true;
			}
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

		bool exitButtonHeldLongEnough()
		{
			return escape_timer_ >= ESCAPE_TIMER_THRESHOLD;
		}
};
