#include "config.hpp"
#include "controls.hpp"
#include "game.hpp"
#include "ow_level_menu.hpp"

namespace BSL
{
    static constexpr int MENUX = static_cast<int> ( ( WINDOW_WIDTH_PIXELS - 160 ) / 2.0 );
    static constexpr int MENUY = static_cast<int> ( ( WINDOW_HEIGHT_PIXELS - 32 ) / 2.0 );

    static constexpr uint_fast8_t SELECTION_PLAY = 0;
    static constexpr uint_fast8_t SELECTION_CANCEL = 1;

    void OWLevelMenu::init( uint_fast8_t lvid )
    {
        levelid = lvid;
        selection = 0;
        BSL::GFX::addGraphicMenu
        (
            160,
            32,
            MENUX,
            MENUY
        );
        gfx.selection = BSL::GFX::addGraphicRect
        (
            MENUX + 5,
            MENUY + 5,
            149,
            11,
            1,
            {
                { "abs", true },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        gfx.optiontext[ SELECTION_PLAY ] = BSL::GFX::addGraphicText
        (
            "Start Level",
            {
                { "x", MENUX + 8 },
                { "y", MENUY + 8 },
                { "color", 0xFF },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        gfx.optiontext[ SELECTION_CANCEL ] = BSL::GFX::addGraphicText
        (
            "Cancel",
            {
                { "x", MENUX + 8 },
                { "y", MENUY + 16 },
                { "color", 0x01 },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        confirmlock = true;
    };

    void OWLevelMenu::update( float dt )
    {
        if ( !BSL::Controls::heldConfirm() )
        {
            confirmlock = false;
        }

        if ( BSL::Controls::heldCancel() )
        {
            updateSelector( SELECTION_CANCEL );
            BSL::Game::popState();
        }
        else if ( !confirmlock && BSL::Controls::heldConfirm() )
        {
            switch ( selection )
            {
                case ( SELECTION_PLAY ):
                {
                    BSL::Game::FadeToArgs args;
                    args.level.levelid = levelid;
                    BSL::Game::fadeTo( &BSL::Game::pushLvMessageState, args );
                }
                break;
                case ( SELECTION_CANCEL ):
                {
                    BSL::Game::popState();
                }
                break;
            }
        }
        else if ( BSL::Controls::heldDown() )
        {
            if ( selection < OPTIONCOUNT - 1 )
            {
                updateSelector( selection + 1 );
            }
        }
        else if ( BSL::Controls::heldUp() )
        {
            if ( selection > 0 )
            {
                updateSelector( selection - 1 );
            }
        }
    };

    void OWLevelMenu::updateSelector( uint_fast8_t newselection )
    {
        // Remove highlight from prev selection text.
        gfx.optiontext[ selection ].setColor( 0x01 );

        // Update selection.
        selection = newselection;

        // Move selection bar bg.
        gfx.selection.setY( MENUY + 5 + ( 11 * selection ) );

        // Set highlight for new selection text.
        gfx.optiontext[ selection ].setColor( 0xFF );
    };
};