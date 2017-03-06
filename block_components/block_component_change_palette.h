




// Name
//===================================
//
// BlockComponentChangePalette
//

#ifndef BLOCK_COMPONENT_CHANGE_PALETTE_H
#define BLOCK_COMPONENT_CHANGE_PALETTE_H


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
    #include "palette.h"


// CLASS
//===================================

    class BlockComponentChangePalette : public BlockComponent
    {
        public:
            BlockComponentChangePalette( const Palette& palette );
            ~BlockComponentChangePalette();
            void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );

        private:
            const Palette palette_;
            bool used_;
    };

#endif // BLOCK_COMPONENT_CHANGE_PALETTE_H

