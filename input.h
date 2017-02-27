

// Name
//===================================
//
// Input
//

#ifndef INPUT_H
#define INPUT_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include <map>
    #include <SDL2/SDL.h>


// CLASS
//===================================

    class Input
    {
        public:
            enum Action
            {
                CONFIRM,
                CANCEL,
                MENU,
                JUMP,
                RUN,
                MOVE_UP,
                MOVE_RIGHT,
                MOVE_DOWN,
                MOVE_LEFT,
                CAMERA_LEFT,
                CAMERA_RIGHT,
                CAMERA_UP,
                CAMERA_DOWN,
                ESCAPE
            };
            static constexpr int NUM_O_ACTIONS = (int)Action::ESCAPE + 1;

			Input();
            void update();
            void reset();
            bool pressed( Action action ) const;
			bool pressedMain() const;
            bool released( Action action ) const;
            bool held( Action action ) const;

            void keyPress( SDL_Keycode key );
            void keyRelease( SDL_Keycode key );
            void keyHold( SDL_Keycode key );


        private:
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
			
			void resetList( std::map<SDL_Keycode, bool>& list );
    };

#endif // INPUT_H

