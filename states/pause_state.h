



// Name
//===================================
//
// PauseState
//

#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H


// FORWARD DECLARATIONS
//===================================

    class InventoryLevel;


// DEPENDENCIES
//===================================

    #include "event_system.h"
    #include "game_state.h"
    #include "text.h"


// CLASS
//===================================

    class PauseState : public GameState
    {

        public:
            PauseState( Palette::PaletteSet palette, EventSystem& events, bool beaten );
            ~PauseState();

            void update( Game& game, Input& input, Graphics& graphics );
            void stateRender( Graphics& graphics );
            void init( Game& game, Graphics& graphics );
            void backFromPop( Game& game, Graphics& graphics );

        private:
            enum class PauseOption
            {
                PO_CONTINUE,
                PO_QUIT,
                PO_LIMIT
            };

            static const int PAUSE_BOX_WIDTH = 24;
            static const int PAUSE_BOX_HEIGHT = 7;
            sdl2::SDLRect surface_box_ =
            {
                Unit::MiniBlocksToPixels( floor( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_WIDTH / 2 ) ),
                Unit::MiniBlocksToPixels( floor( Unit::WINDOW_HEIGHT_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_HEIGHT / 2 ) ),
                Unit::MiniBlocksToPixels( PAUSE_BOX_WIDTH ),
                Unit::MiniBlocksToPixels( PAUSE_BOX_HEIGHT )
            };
            Text option_text_[ (int)PauseOption::PO_LIMIT ] =
            {
                Text( "Continue", surface_box_.x + 8, surface_box_.y + 16, Text::FontShade::LIGHT_MID_GRAY ),
                Text( "Back to Level Select", surface_box_.x + 8, surface_box_.y + 32, Text::FontShade::LIGHT_MID_GRAY )
            };
            PauseOption option_selection_ = PauseOption::PO_CONTINUE;
            EventSystem& events_;
    };

#endif // PAUSE_STATE_H
