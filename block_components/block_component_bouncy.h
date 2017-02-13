



// Name
//===================================
//
// BlockComponentBouncy
//

#ifndef BLOCK_COMPONENT_BOUNCY_H
#define BLOCK_COMPONENT_BOUNCY_H


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

    class BlockComponentBouncy : public BlockComponent
    {
        public:
            BlockComponentBouncy();
            ~BlockComponentBouncy();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_BOUNCY_H

