





// Name
//===================================
//
// BlockConditionRival
//

#ifndef BLOCK_CONDITION_RIVAL_H
#define BLOCK_CONDITION_RIVAL_H


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

    class BlockConditionRival : public BlockCondition
    {
        public:
            BlockConditionRival();
            ~BlockConditionRival();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_RIVAL_H
