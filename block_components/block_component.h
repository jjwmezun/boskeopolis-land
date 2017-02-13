

// Name
//===================================
//
// BlockComponent
//

#ifndef BLOCK_COMPONENT_H
#define BLOCK_COMPONENT_H


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


// CLASS
//===================================

    class BlockComponent
    {
        public:
            BlockComponent();
            virtual ~BlockComponent();
            virtual void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) = 0;
            virtual void update( EventSystem& events, BlockType& type );
    };

#endif // BLOCK_COMPONENT_H

