


// Name
//===================================
//
// BlockComponentSolid
//

#ifndef BLOCK_COMPONENT_SOLID_H
#define BLOCK_COMPONENT_SOLID_H


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

    class BlockComponentSolid : public BlockComponent
    {
        public:
            BlockComponentSolid();
            ~BlockComponentSolid();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_SOLID_H

