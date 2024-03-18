#include "config.hpp"
#include "level_inventory.hpp"

namespace BSL
{
    void LevelInventory::init()
    {
        pts = pts_shown = 0;
        prev_hp = hp = 30.0f;
        resetGFX();
    };

    void LevelInventory::resetGFX()
    {
        BSL::GFX::addGraphicMenu( WINDOW_WIDTH_PIXELS, 40, 0, WINDOW_HEIGHT_PIXELS - 40 );
        pts_gfx = BSL::GFX::addGraphicCounter
        (
            pts,
            5,
            {
                { "x", 8 + 4 + 32 },
                { "y", 256 + 4 },
                { "color", 0x01 },
                { "prefix", "₧" }
            }
        );

        hp_gfx = BSL::GFX::addGraphicCounter
        (
            static_cast<int_fast32_t> ( hp ),
            2,
            {
                { "x", 8 + 4 },
                { "y", 256 + 4 },
                { "color", 0x01 },
                { "prefix", "♥" }
            }
        );
    };

    void LevelInventory::update( float dt )
    {
        // Increment pts_shown till it reaches pts.
        if ( pts_shown < pts )
        {
            pts_shown += 5.0f * dt;
            if ( pts_shown > pts )
            {
                pts_shown = pts;
            }
            pts_gfx.changeNumber( static_cast<int_fast32_t> ( pts_shown ) );
        }

        if ( hp != prev_hp )
        {
            hp_gfx.changeNumber( static_cast<int_fast32_t> ( hp ) );
            prev_hp = hp;
        }
    };

    void LevelInventory::addMoney( uint_fast32_t amount )
    {
        pts += amount;
        if ( pts > 99999 )
        {
            pts = 99999;
        }
    };

    void LevelInventory::hurt( float amount )
    {
        hp -= amount;
        if ( hp < 0.0f )
        {
            hp = 0.0f;
        }
    };
};