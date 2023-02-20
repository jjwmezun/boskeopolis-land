#ifndef LEVEL_H
#define LEVEL_H

#include "level_inventory.hpp"
#include "map.hpp"
#include "sprite.hpp"

namespace BSL
{
    class Game;

    class Level
    {
        public:
            inline Level()
            :
                map_ ( "city-1" ),
                autumn_ ()
            {};
            void init( Game & game );
            void update( Game & game, const Controller & controller, float dt );

            inline Map & map() { return map_; };
            inline LevelInventory & inventory() { return inventory_; };

        private:
            Map map_;
            Sprite autumn_;
            LevelInventory inventory_;
    };
}

#endif // LEVEL_H