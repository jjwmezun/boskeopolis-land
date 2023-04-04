#ifndef BLOCK_BEHAVIOR_H
#define BLOCK_BEHAVIOR_H

#include "block_component.hpp"

namespace BSL
{
    class Block;
    class Game;
    class Level;
    class Map;
    class Sprite;

    class BlockBehavior
    {
        public:
            inline BlockBehavior( const BlockComponent & component )
                : component_ ( component )
            {};

            void interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const;

        private:
            const BlockComponent component_;
    };
}

#endif // BLOCK_BEHAVIOR_H