




// Name
//===================================
//
// BlockComponentKey
//

#ifndef BLOCK_COMPONENT_KEY_H
#define BLOCK_COMPONENT_KEY_H


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

    class BlockComponentKey : public BlockComponent
    {
        public:
            BlockComponentKey();
            ~BlockComponentKey();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_KEY_H

