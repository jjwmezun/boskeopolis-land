#ifndef LEVEL_INVENTORY_H
#define LEVEL_INVENTORY_H

#include <cstdint>
#include "gfx.hpp"

namespace BSL
{
    struct LevelInventory
    {
        BSL::GFX::Counter pts_gfx;
        BSL::GFX::Counter hp_gfx;
        uint_fast32_t pts;
        float pts_shown;
        float hp;
        float prev_hp;

        void init();
        void update( float dt );
        void addMoney( uint_fast32_t amount );
        void hurt( float amount );
    };
};

#endif // LEVEL_INVENTORY_H