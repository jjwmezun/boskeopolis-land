#include "config.hpp"
#include "level_inventory.hpp"

#include "game.hpp"

namespace BSL
{
    void LevelInventory::init( Game & game )
    {
        const float y = WINDOW_HEIGHT_PIXELS - 32.0f;
        ArgList text_args = 
        {
            { "num", 0.0f },
            { "maxdigits", 5 },
            { "x", 16.0f },
            { "y", y},
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

        // Render HP
        for ( unsigned int i = 0; i < hp_; ++i )
        {
            hp_gfx_.emplace_back( game.render().addSprite( "misc/inventory.png", 0.0f, 0.0f, 8.0f, 8.0f, x, y, { { "abs", true }, { "layer", Layer::AFTER_FG_2 } } ) );
            x += 8.0f;
        }
        x += 8.0f;

        // Render funds.
        text_args[ "x" ] = x;
        text_args[ "text" ] = "₧";
        game.render().addText( text_args );
        x += 8.0f;
        text_args[ "x" ] = x;
        funds_gfx_ = game.render().addCounter( text_args );

        // Render clock.
        x += ( 8.0f * 6 );
        text_args[ "x" ] = x;
        clock_.init( game, text_args );

        x += ( 8.0f * 6 );
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

    void LevelInventory::hurt()
    {
        if ( hp_ > 0 )
        {
            --hp_;
            hp_gfx_[ hp_ ].setSrcX( 8.0f );
        }
    };

    bool LevelInventory::isDead() const { return hp_ == 0; };
}