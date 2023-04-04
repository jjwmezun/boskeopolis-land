#ifndef BLOCK_TYPE_H
#define BLOCK_TYPE_H

#include "block_behavior.hpp"
#include <string>
#include <vector>

namespace BSL
{
    class Block;
    class Game;
    class Level;
    class Map;
    class Sprite;

    class BlockType
    {
        public:
            BlockType();
            BlockType
            (
                unsigned char x,
                unsigned char y,
                unsigned char animation,
                const std::vector<BlockBehavior> & behaviors
            );
            bool isNull() const;
            unsigned char getX() const;
            unsigned char getY() const;
            unsigned char getAnimation() const;
            void interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const;

        private:
            bool is_null_;
            unsigned char x_;
            unsigned char y_;
            unsigned char animation_;
            std::vector<BlockBehavior> behaviors_;
    };
}

#endif // BLOCK_TYPE_H