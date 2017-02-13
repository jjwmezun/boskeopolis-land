



// Name
//===================================
//
// BlockComponentLowSlopeLeft
//

#ifndef BLOCK_COMPONENT_LOW_SLOPE_LEFT_H
#define BLOCK_COMPONENT_LOW_SLOPE_LEFT_H


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

    class BlockComponentLowSlopeLeft : public BlockComponent
    {
        public:
            BlockComponentLowSlopeLeft();
            ~BlockComponentLowSlopeLeft();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_LOW_SLOPE_LEFT_H

