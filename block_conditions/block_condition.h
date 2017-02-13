


// Name
//===================================
//
// BlockCondition
//

#ifndef BLOCK_CONDITION_H
#define BLOCK_CONDITION_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Collision;
    class EventSystem;
    class Sprite;


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class BlockCondition
    {
        public:
            BlockCondition();
            virtual ~BlockCondition();
            virtual bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) = 0;
    };

#endif // BLOCK_CONDITION_H

