#ifndef LEVEL_INVENTORY_H
#define LEVEL_INVENTORY_H

#include <cstdint>
#include "gfx.hpp"

namespace BSL
{
    struct LevelInventory
    {
        BSL::GFX::Counter pts_gfx;
        uint_fast32_t pts;
        float pts_shown;

        void init();
        void update( float dt );
        void addMoney( uint_fast32_t amount );
    };
};

#endif // LEVEL_INVENTORY_H