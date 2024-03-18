#include "config.hpp"
#include "controls.hpp"
#include "game.hpp"
#include "level_pause.hpp"

namespace BSL
{
    static constexpr int MENUX = static_cast<int> ( ( WINDOW_WIDTH_PIXELS - 160 ) / 2.0 );
    static constexpr int MENUY = static_cast<int> ( ( WINDOW_HEIGHT_PIXELS - 32 ) / 2.0 );

    static constexpr uint_fast8_t SELECTION_CONTINUE = 0;
    static constexpr uint_fast8_t SELECTION_EXIT = 1;

    void LevelPause::init()
    {
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
        gfx.optiontext[ SELECTION_CONTINUE ] = BSL::GFX::addGraphicText
        (
            "Continue",
            {
                { "x", MENUX + 8 },
                { "y", MENUY + 8 },
                { "color", 0xFF },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        gfx.optiontext[ SELECTION_EXIT ] = BSL::GFX::addGraphicText
        (
            "Exit to O’erworld",
            {
                { "x", MENUX + 8 },
                { "y", MENUY + 16 },
                { "color", 0x01 },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        confirmlock = true;
    };

    void LevelPause::update( float dt )
    {
        if ( !BSL::Controls::heldConfirm() )
        {
            confirmlock = false;
        }

        if ( BSL::Controls::heldCancel() )
        {
            updateSelector( SELECTION_EXIT );
            BSL::Game::popState();
        }
        else if ( !confirmlock && BSL::Controls::heldConfirm() )
        {
            switch ( selection )
            {
                case ( SELECTION_CONTINUE ):
                {
                    BSL::Game::popState();
                }
                break;
                case ( SELECTION_EXIT ):
                {
                    BSL::Game::fadeToOW( { 0, 0, 0 } );
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

    void LevelPause::updateSelector( uint_fast8_t newselection )
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