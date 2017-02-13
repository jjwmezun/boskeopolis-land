




// Name
//===================================
//
// BlockComponentHighSlopeLeft
//

#ifndef BLOCK_COMPONENT_HIGH_SLOPE_LEFT_H
#define BLOCK_COMPONENT_HIGH_SLOPE_LEFT_H


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

    class BlockComponentHighSlopeLeft : public BlockComponent
    {
        public:
            BlockComponentHighSlopeLeft();
            ~BlockComponentHighSlopeLeft();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_HIGH_SLOPE_LEFT_H

