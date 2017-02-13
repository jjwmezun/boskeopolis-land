


// Name
//===================================
//
// BlockConditionCollideLeft
//

#ifndef BLOCK_CONDITION_COLLIDE_LEFT_H
#define BLOCK_CONDITION_COLLIDE_LEFT_H


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

    class BlockConditionCollideLeft : public BlockCondition
    {
        public:
            BlockConditionCollideLeft();
            ~BlockConditionCollideLeft();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_COLLIDE_LEFT_H
