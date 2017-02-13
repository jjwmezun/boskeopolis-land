




// Name
//===================================
//
// BlockComponentLowSlopeRight
//

#ifndef BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H
#define BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H


// FORWARD DECLARATIONS
//===================================

    class Block;
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

    class BlockComponentLowSlopeRight : public BlockComponent
    {
        public:
            BlockComponentLowSlopeRight();
            ~BlockComponentLowSlopeRight();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H

