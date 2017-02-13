



// Name
//===================================
//
// BlockComponentChangeBlock
//

#ifndef BLOCK_COMPONENT_CHANGE_BLOCK_H
#define BLOCK_COMPONENT_CHANGE_BLOCK_H


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

    class BlockComponentChangeBlock : public BlockComponent
    {
        public:
            BlockComponentChangeBlock( int new_block );
            ~BlockComponentChangeBlock();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& InventoryLevel, Camera& camera );

        private:
            int new_block_;
    };

#endif // BLOCK_COMPONENT_CHANGE_BLOCK_H

