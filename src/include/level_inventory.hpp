#ifndef LEVEL_INVENTORY_H
#define LEVEL_INVENTORY_H

#include "clock.hpp"
#include <cstdint>
#include "ranged_float.hpp"
#include <vector>

namespace BSL
{
    class Game;

    class LevelInventory
    {
        public:
            inline LevelInventory()
            :
                clock_ ( 0 ),
                funds_gfx_ ( 0 ),
                funds_ ( 99999.0f, 0.0f, 0.0f ),
                funds_show_ ( 0.0f ),
                hp_ ( 2 )
            {};
            void init( Game & game );
            void update( float dt );
            void addFunds( float amount );
            void hurt();
            bool isDead() const;

        private:
            Clock clock_;
            std::vector<SpriteGraphic> hp_gfx_;
            CounterGraphic funds_gfx_;
            RangedFloat funds_;
            float funds_show_;
            uint_fast8_t hp_;
    };
}

#endif // LEVEL_INVENTORY_H