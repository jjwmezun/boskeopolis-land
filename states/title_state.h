




// Name
//===================================
//
// TitleState
//

#ifndef TITLE_STATE_H
#define TITLE_STATE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "game_state.h"
    #include "sprite_graphics.h"
    #include "text.h"
    #include <array>


// CLASS
//===================================

    class TitleState : public GameState
    {

        public:
            TitleState();
            ~TitleState();

            void update( Game& game, Input& input, Graphics& graphics );
            void stateRender( Graphics& graphics );
            void init( Game& game, Graphics& graphics );
            void backFromPop( Game& game, Graphics& graphics );

        private:
            enum class Option
            {
                NEW,
                LOAD,
                OPTIONS,
                QUIT,
                __END
            };
            static const int OPTIONS_SIZE = (int)Option::__END;
            static const int OPTIONS_TOP_Y = 64;

            SpriteGraphics logo_gfx_ = SpriteGraphics( Graphics::SpriteSheet::BOSKO_LOGO );
            sdl2::SDLRect logo_rect_ = { 0, 0, 320, 48 };
            sdl2::SDLRect highlight_rect_ = { 16, OPTIONS_TOP_Y, Unit::WINDOW_WIDTH_PIXELS - 32, 8 };

            const std::array<const std::string, (std::size_t)OPTIONS_SIZE> option_text_ =
            {{
                "New Game",
                "Load Game",
                "Options",
                "Quit"
            }};
            Counter selection_ = { 0, OPTIONS_SIZE - 1 };

            bool can_load_ = false;
    };

#endif // TITLE_STATE_H
