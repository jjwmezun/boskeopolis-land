



// Name
//===================================
//
// BlockComponentHeal
//

#ifndef BLOCK_COMPONENT_HEAL_H
#define BLOCK_COMPONENT_HEAL_H


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

    class BlockComponentHeal : public BlockComponent
    {
        public:
            BlockComponentHeal( int amount = 1 );
            ~BlockComponentHeal();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );

        private:
            int amount_;
    };

#endif // BLOCK_COMPONENT_HEAL_H

