


// Name
//===================================
//
// BlockConditionCollideAny
//

#ifndef BLOCK_CONDITION_COLLIDE_ANY_H
#define BLOCK_CONDITION_COLLIDE_ANY_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Collision;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "block_condition.h"


// CLASS
//===================================

    class BlockConditionCollideAny : public BlockCondition
    {
        public:
            BlockConditionCollideAny();
            ~BlockConditionCollideAny();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_COLLIDE_ANY_H


