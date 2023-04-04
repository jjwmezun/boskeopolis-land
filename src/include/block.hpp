#ifndef BLOCK_H
#define BLOCK_H

#include "block_type.hpp"
#include <functional>

namespace BSL
{
    class Game;
    class Level;
    class Map;
    class Sprite;

    class Block
    {
        public:
            constexpr Block( unsigned int layer, const BlockType & type )
                : layer_ ( layer ), type_ ( type ) {};
            inline void interact( Sprite & sprite, Level & level, Game & game, Map & map )
            {
                type_.get().interact( sprite, level, game, map, *this );
            };
            constexpr unsigned int getLayer() const
            {
                return layer_;
            }
        private:
            unsigned int layer_;
            std::reference_wrapper<const BlockType> type_;
    };
}

#endif // BLOCK_H