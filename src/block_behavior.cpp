#include "block_behavior.hpp"

namespace BSL
{
    void BlockBehavior::interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const
    {
        component_.interact( sprite, level, game, map, block );
    };
}