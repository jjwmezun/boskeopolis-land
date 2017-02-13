


// Name
//===================================
//
// BlockConditionCollideRight
//

#ifndef BLOCK_CONDITION_COLLIDE_RIGHT_H
#define BLOCK_CONDITION_COLLIDE_RIGHT_H


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

    class BlockConditionCollideRight : public BlockCondition
    {
        public:
            BlockConditionCollideRight();
            ~BlockConditionCollideRight();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_COLLIDE_RIGHT_H
