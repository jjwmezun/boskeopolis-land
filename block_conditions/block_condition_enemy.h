




// Name
//===================================
//
// BlockConditionEnemy
//

#ifndef BLOCK_CONDITION_ENEMY_H
#define BLOCK_CONDITION_ENEMY_H


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

    class BlockConditionEnemy : public BlockCondition
    {
        public:
            BlockConditionEnemy();
            ~BlockConditionEnemy();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_ENEMY_H
