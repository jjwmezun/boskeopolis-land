






// Name
//===================================
//
// BlockComponentSwitchFlip
//

#ifndef BLOCK_COMPONENT_SWITCH_FLIP_H
#define BLOCK_COMPONENT_SWITCH_FLIP_H


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
    #include <memory>
    #include "timers/timer_simple.h"


// CLASS
//===================================

    class BlockComponentSwitchFlip : public BlockComponent
    {
        public:
            BlockComponentSwitchFlip();
            ~BlockComponentSwitchFlip();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
            void update( EventSystem& events, BlockType& type );

        private:
            TimerSimple delay_;
            bool ready_ = true;
    };

#endif // BLOCK_COMPONENT_SWITCH_FLIP_H

