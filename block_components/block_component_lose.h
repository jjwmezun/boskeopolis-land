





// Name
//===================================
//
// BlockComponentLose
//

#ifndef BLOCK_COMPONENT_LOSE_H
#define BLOCK_COMPONENT_LOSE_H


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


// CLASS
//===================================

    class BlockComponentLose : public BlockComponent
    {
        public:
            BlockComponentLose();
            ~BlockComponentLose();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_LOSE_H

