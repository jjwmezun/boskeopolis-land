






// Name
//===================================
//
// MessageState
//

#ifndef MESSAGE_STATE_H
#define MESSAGE_STATE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "game_state.h"
    #include "text.h"


// CLASS
//===================================

    class MessageState : public GameState
    {

        public:
            MessageState
            (
                std::string message,
                bool pop = false,
                std::unique_ptr<GameState> next_state = nullptr,
                bool push = false,
                Palette::PaletteSet palette = { Palette::PaletteType::GRAYSCALE, 1 },
                Text::FontShade font_color = Text::FontShade::BLACK
            );
            ~MessageState();

            void update( Game& game, Input& input, Graphics& graphics );
            void stateRender( Graphics& graphics );
            void init( Game& game, Graphics& graphics );
            void backFromPop( Game& game, Graphics& graphics );

        private:
            const Text message_;
            const bool pop_;
            std::unique_ptr<GameState> next_state_;
            const bool push_;
            const Text::FontShade font_color_;
    };

#endif // MESSAGE_STATE_H
