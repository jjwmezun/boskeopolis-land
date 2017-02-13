





// Name
//===================================
//
// BlockComponentSwitch
//

#ifndef BLOCK_COMPONENT_SWITCH_H
#define BLOCK_COMPONENT_SWITCH_H


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
    #include "sprite_graphics.h"
    #include <vector>


// CLASS
//===================================

    class BlockComponentSwitch : public BlockComponent
    {
        public:
            BlockComponentSwitch
            (
                BlockComponent* off_component,
                BlockComponent* on_component,
                std::vector<SpriteGraphics*> off_graphics,
                std::vector<SpriteGraphics*> on_graphics
            );
            ~BlockComponentSwitch();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
            void update( EventSystem& events, BlockType& type );


        private:
            std::unique_ptr<BlockComponent> off_component_;
            std::unique_ptr<BlockComponent> on_component_;
            std::vector<std::shared_ptr<SpriteGraphics>> on_graphics_;
            std::vector<std::shared_ptr<SpriteGraphics>> off_graphics_;

            void changeGraphics( BlockType& type, std::vector<std::shared_ptr<SpriteGraphics>>& graphics );
            bool first_update_ = true;
    };

#endif // BLOCK_COMPONENT_SWITCH_H

