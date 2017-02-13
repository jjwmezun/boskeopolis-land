



// Name
//===================================
//
// BlockComponentDropping
//

#ifndef BLOCK_COMPONENT_DROPPING_H
#define BLOCK_COMPONENT_DROPPING_H


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

    class BlockComponentDropping : public BlockComponent
    {
        public:
            BlockComponentDropping();
            ~BlockComponentDropping();
            void interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
    };

#endif // BLOCK_COMPONENT_DROPPING_H


