





// Name
//===================================
//
// BlockComponentConveyor
//

#ifndef BLOCK_COMPONENT_CONVEYOR_H
#define BLOCK_COMPONENT_CONVEYOR_H


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
    #include "direction.h"


// CLASS
//===================================

    class BlockComponentConveyor : public BlockComponent
    {
        public:
            BlockComponentConveyor( Direction::Horizontal direction, int speed = 1000 );
            ~BlockComponentConveyor();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );

        private:
            int x_effect_;
    };

#endif // BLOCK_COMPONENT_CONVEYOR_H

