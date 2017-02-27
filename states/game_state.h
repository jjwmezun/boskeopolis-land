

// Name
//===================================
//
// GameState
//


#ifndef GAME_STATE_H
#define GAME_STATE_H


// FORWARD DECLARATIONS
//===================================

    class Game;
    class Graphics;
	class Input;


// DEPENDENCIES
//===================================

    #include "camera.h"
    #include "palette.h"


// CLASS
//===================================

    class GameState
    {
        public:
            enum StateID
            {
                __NULL,
                TITLE_STATE,
                LEVEL_SELECT_STATE,
                LEVEL_STATE,
                MESSAGE_STATE,
                PAUSE_STATE
            };

            GameState
            (
                StateID id = __NULL, Palette::PaletteSet palette = { Palette::PaletteType::GRAYSCALE, 1 }
            );
            virtual ~GameState();

            void render( Graphics& graphics );

            virtual void update( Game& game, const Input& input, Graphics& graphics ) = 0;
            virtual void stateRender( Graphics& graphics ) = 0;
            virtual void init( Game& game, Graphics& graphics ) = 0;
            virtual void backFromPop( Game& game, Graphics& graphics ) = 0;
            void changePalette( Graphics& graphics );
            void newPalette( Graphics& graphics, Palette::PaletteSet pallete );
            StateID id() const;

        protected:
            const StateID id_;

        private:
            Palette::PaletteSet palette_;
    };

#endif // GAME_STATE_H
