#include "audio.hpp"
#include <cassert>
#include "config.hpp"
#include <fstream>
#include "input.hpp"
#include <iostream>
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "message_state.hpp"
#include "mezun_helpers.hpp"
#include "mezun_json.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <vector>
#include "wtext_obj.hpp"

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

		static std::u32string key_names_[ NUM_O_ACTIONS ] = { U"" };
		static std::u32string button_names_[ NUM_O_ACTIONS ] = { U"" };

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
		static WTextObj quitting_text;

		static bool letters_[ NUMBER_OF_LETTERS ];
		static bool letters_lock_[ NUMBER_OF_LETTERS ];
		static SDL_Keycode letter_keys_[ NUMBER_OF_LETTERS ] =
		{
			SDLK_a,
			SDLK_b,
			SDLK_c,
			SDLK_d,
			SDLK_e,
			SDLK_f,
			SDLK_g,
			SDLK_h,
			SDLK_i,
			SDLK_j,
			SDLK_k,
			SDLK_l,
			SDLK_m,
			SDLK_n,
			SDLK_o,
			SDLK_p,
			SDLK_q,
			SDLK_r,
			SDLK_s,
			SDLK_t,
			SDLK_u,
			SDLK_v,
			SDLK_w,
			SDLK_x,
			SDLK_y,
			SDLK_z,
			SDLK_SPACE,
			SDLK_MINUS
		};
		static char32_t letter_characters_[ NUMBER_OF_LETTERS ] =
		{
			u'A',
			u'B',
			u'C',
			u'D',
			u'E',
			u'F',
			u'G',
			u'H',
			u'I',
			u'J',
			u'K',
			u'L',
			u'M',
			u'N',
			u'O',
			u'P',
			u'Q',
			u'R',
			u'S',
			u'T',
			u'U',
			u'V',
			u'W',
			u'X',
			u'Y',
			u'Z',
			u' ',
			u'-'
		};

	//
	//  PRIVATE FUNCTION DECLARATIONS
	//
	//////////////////////////////////////////////////////////

		static void loadJoysticks();
		static void loadConfig();
		static void loadConfigFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& document_object );
		static void saveConfig();
		static std::string getConfigFilename();
		static void resetList( bool* list );
		static void updateEscape();
		static void registerKeyPress( Action action );
		static void registerKeyRelease( Action action );
		static void registerKeyHold( Action action );
		static void registerAxisPress( Uint8 axis, Sint16 value );
		static void registerSingleAxisPress( Sint16 value, Uint8 negative, Uint8 positive );
		static void registerAxisChange( Uint8 axis, Sint16 value );
		static bool movingCharacterFunction( bool ( *f )( Action a ) );
		static void setKeycodeChangeFinish( SDL_Keycode key );
		static void setButtonChangeFinish( Uint8 button );
		static void setPreferedButtonConfig();
		static std::string getButtonName( Uint8 button );
		static void resetLetters();


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
			changeQuittingText();
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
					const char* input_name = TAGS[ i ];
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
			if ( document_object.HasMember( "controls" ) && document_object[ "controls" ].IsObject() )
			{
				const auto& controls = document_object[ "controls" ].GetObject();
				loadConfigType( controls, "keys", key_map_ );
				loadConfigType( controls, "buttons", controller_button_map_ );
			}
			else
			{
				saveConfig();
				Main::pushState( std::unique_ptr<MessageState> ( MessageState::errorMessage( "Input configuration file has been corruped.\nConfig file has been replaced with default." ) ) );
			}
		};

		void saveConfig()
		{
			rapidjson::Document document = Config::readData();

			rapidjson::Value keys;
			keys.SetObject();
			rapidjson::Value buttons;
			buttons.SetObject();
			for ( int i = 0; i < NUM_O_ACTIONS; ++i )
			{
				rapidjson::Value key_name;
				std::string key_name_string = TAGS[ i ];
				key_name.SetString( key_name_string.data(), key_name_string.size(), document.GetAllocator() );
				rapidjson::Value key;
				key.SetInt( ( int )( key_map_[ i ] ) );
				keys.AddMember( key_name, key, document.GetAllocator() );

				rapidjson::Value button_name;
				std::string button_name_string = TAGS[ i ];
				button_name.SetString( button_name_string.data(), button_name_string.size(), document.GetAllocator() );
				rapidjson::Value button;
				button.SetInt( ( int )( controller_button_map_[ i ] ) );
				buttons.AddMember( button_name, button, document.GetAllocator() );
			}

			if ( document.HasMember( "controls" ) )
			{
				document[ "controls" ].SetObject();
				document[ "controls" ].AddMember( "keys", keys, document.GetAllocator() );
				document[ "controls" ].AddMember( "buttons", buttons, document.GetAllocator() );
			}
			else
			{
				rapidjson::Value controls;
				controls.SetObject();
				controls.AddMember( "keys", keys, document.GetAllocator() );
				controls.AddMember( "buttons", buttons, document.GetAllocator() );
				document.AddMember( "controls", controls, document.GetAllocator() );
			}

			Config::saveData( document );
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
			resetLetters();
		};

		void renderQuitText()
		{
			if ( escape_timer_ > 0 )
			{
				quitting_text.render();
			}
		};

		void update()
		{
			updateEscape();
			resetList( actions_pressed_ );
			resetList( actions_released_ );
			for ( int i = 0; i < NUMBER_OF_LETTERS; ++i )
			{
				const SDL_Keycode k = letter_keys_[ i ];
				letters_[ i ] = false;
			}
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

		const std::u32string* generateKeyNames()
		{
			for ( int i = 0; i < NUM_O_ACTIONS; ++i )
			{
				SDL_Keycode key = key_map_[ i ];
				key_names_[ i ] = mezun::charToChar32String( SDL_GetKeyName( key ) );
			}
			return key_names_;
		};

		const std::u32string* generateButtonNames()
		{
			for ( int i = 0; i < NUM_O_ACTIONS; ++i )
			{
				Uint8 button = controller_button_map_[ i ];
				button_names_[ i ] = mezun::charToChar32String( getButtonName( button ).c_str() );
			}
			return button_names_;
		};

		std::string getButtonName( Uint8 button )
		{
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
				for ( int i = 0; i < NUMBER_OF_LETTERS; ++i )
				{
					const SDL_Keycode k = letter_keys_[ i ];
					if ( k == key && !letters_lock_[ i ] )
					{
						letters_[ i ] = true;
						letters_lock_[ i ] = true;
					}
				}
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
				for ( int i = 0; i < NUMBER_OF_LETTERS; ++i )
				{
					const SDL_Keycode k = letter_keys_[ i ];
					if ( k == key )
					{
						letters_[ i ] = false;
						letters_lock_[ i ] = false;
					}
				}
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

		void changeQuittingText()
		{
			quitting_text = WTextObj
			(
				Localization::getCurrentLanguage().getInputQuitting(),
				8,
				Unit::WINDOW_HEIGHT_PIXELS - 16,
				WTextCharacter::Color::WHITE,
				WTextObj::DEFAULT_WIDTH,
				WTextObj::Align::LEFT,
				WTextCharacter::Color::BLACK
			);
		}

		const bool* getLetters()
		{
			return &letters_[ 0 ];
		};

		void resetLetters()
		{
			for ( int i = 0; i < NUMBER_OF_LETTERS; ++i )
			{
				const SDL_Keycode k = letter_keys_[ i ];
				letters_[ i ] = false;
				letters_lock_[ i ] = false;
			}
		}

		char32_t getLetterCharacter( int i )
		{
			return letter_characters_[ i ];
		}
};
