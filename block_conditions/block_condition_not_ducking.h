




// Name
//===================================
//
// BlockConditionNotDucking
//

#ifndef BLOCK_CONDITION_NOT_DUCKING_H
#define BLOCK_CONDITION_NOT_DUCKING_H


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

    class BlockConditionNotDucking : public BlockCondition
    {
        public:
            BlockConditionNotDucking();
            ~BlockConditionNotDucking();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_NOT_DUCKING_H
