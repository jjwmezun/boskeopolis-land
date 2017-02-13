



// Name
//===================================
//
// BlockComponentChangeDirection
//

#ifndef BLOCK_COMPONENT_CHANGE_DIRECTION_H
#define BLOCK_COMPONENT_CHANGE_DIRECTION_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class BlockType;
    class Camera;
    class Collision;
    class EventSystem;
    class InventoryLevel;
    class GameState;
    class Level;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "block_component.h"
    #include "direction.h"


// CLASS
//===================================

    class BlockComponentChangeDirection : public BlockComponent
    {
        public:
            BlockComponentChangeDirection( Direction::Simple direction );
            ~BlockComponentChangeDirection();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );

        private:
            Direction::Simple direction_;
    };

#endif // BLOCK_COMPONENT_CHANGE_DIRECTION_H

