




// Name
//===================================
//
// BlockConditionKey
//

#ifndef BLOCK_CONDITION_KEY_H
#define BLOCK_CONDITION_KEY_H


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

    class BlockConditionKey : public BlockCondition
    {
        public:
            BlockConditionKey( bool must_have_key );
            ~BlockConditionKey();
            bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events );

        private:
            bool must_have_key_;
    };

#endif // BLOCK_CONDITION_KEY_H
