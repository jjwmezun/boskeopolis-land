



// Name
//===================================
//
// BlockConditionCollideTop
//

#ifndef BLOCK_CONDITION_COLLIDE_TOP_H
#define BLOCK_CONDITION_COLLIDE_TOP_H


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

    class BlockConditionCollideTop : public BlockCondition
    {
        public:
            BlockConditionCollideTop();
            ~BlockConditionCollideTop();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_COLLIDE_TOP_H
