


// Name
//===================================
//
// BlockComponentSingleUse
//

#ifndef BLOCK_COMPONENT_SINGLE_USE_H
#define BLOCK_COMPONENT_SINGLE_USE_H


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

    class BlockComponentSingleUse : public BlockComponent
    {
        public:
            BlockComponentSingleUse( int width = 1, int height = 1, int corner = 0 );
            ~BlockComponentSingleUse();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );

        private:
            int width_;
            int height_;
            int corner_;
    };

#endif // BLOCK_COMPONENT_SINGLE_USE_H

