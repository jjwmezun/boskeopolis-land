



// Name
//===================================
//
// BlockConditionHero
//

#ifndef BLOCK_CONDITION_HERO_H
#define BLOCK_CONDITION_HERO_H


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

    class BlockConditionHero : public BlockCondition
    {
        public:
            BlockConditionHero();
            ~BlockConditionHero();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );
    };

#endif // BLOCK_CONDITION_HERO_H
