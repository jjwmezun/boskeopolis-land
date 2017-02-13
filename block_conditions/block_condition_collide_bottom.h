




// Name
//===================================
//
// BlockConditionCollideBottom
//

#ifndef BLOCK_CONDITION_COLLIDE_BOTTOM_H
#define BLOCK_CONDITION_COLLIDE_BOTTOM_H


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

    class BlockConditionCollideBottom : public BlockCondition
    {
        public:
            BlockConditionCollideBottom();
            ~BlockConditionCollideBottom();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_COLLIDE_BOTTOM_H
