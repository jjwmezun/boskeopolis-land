#ifndef BLOCK_COMPONENT_MONEY_H
#define BLOCK_COMPONENT_MONEY_H

#include "block_component.hpp"

namespace BSL
{
    class BlockComponentMoney : public BlockComponent
    {
        public:
            BlockComponentMoney( float amount );
            void interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const override;

        private:
            const float amount_;
    };
}

#endif // BLOCK_COMPONENT_MONEY_H