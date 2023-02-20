#include "config.hpp"
#include "level_inventory.hpp"

#include "game.hpp"

namespace BSL
{
    void LevelInventory::init( Game & game )
    {
        ArgList text_args = 
        {
            { "num", 0.0f },
            { "maxdigits", 5 },
            { "x", 16.0f },
            { "y", WINDOW_HEIGHT_PIXELS - 32.0f },
            { "type", "gradient" },
            { "dir", Dir::XY::DOWN },
            { "color1", 64 },
            { "color2", 16 },
            { "shadow", 0.25f }
        };
        float x = 16.0f;

        // Render box.
        game.render().addMenuBox
        (
            Text::Align::LEFT,
            Text::Valign::TOP,
            static_cast<float>( WINDOW_WIDTH_PIXELS - 16 ),
            32.0f,
            {
                { "x", 8.0f },
                { "y", static_cast<float>( WINDOW_HEIGHT_PIXELS - 40 ) }
            }
        );

        // Render funds.
        text_args[ "text" ] = "₧";
        game.render().addText( text_args );
        x += 8.0f;
        text_args[ "x" ] = x;
        funds_gfx_ = game.render().addCounter( text_args );

        // Render clock.
        x += ( 8.0f * 6 );
        text_args[ "x" ] = x;
        clock_.init( game, text_args );
    };

    void LevelInventory::update( float dt )
    {
        clock_.addSeconds( 1.0f / 60.0f * dt );

        if ( funds_show_ < funds_.get() )
        {
            funds_show_ = std::min( funds_.get(), funds_show_ + ( 10.0f * dt ) );
            funds_gfx_.setNumber( funds_show_ );
        }
    };

    void LevelInventory::addFunds( float amount )
    {
        funds_ += amount;
    };
}