#ifndef LEVEL_INVENTORY_H
#define LEVEL_INVENTORY_H

#include "clock.hpp"
#include "ranged_float.hpp"

namespace BSL
{
    class Game;

    class LevelInventory
    {
        public:
            constexpr LevelInventory()
            :
                clock_ ( 0 ),
                funds_gfx_ ( 0 ),
                funds_ ( 99999.0f, 0.0f, 0.0f ),
                funds_show_ ( 0.0f )
            {};
            void init( Game & game );
            void update( float dt );
            void addFunds( float amount );

        private:
            Clock clock_;
            CounterGraphic funds_gfx_;
            RangedFloat funds_;
            float funds_show_;
    };
}

#endif // LEVEL_INVENTORY_H