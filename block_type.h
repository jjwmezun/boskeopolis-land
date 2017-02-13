


// Name
//===================================
//
// BlockType
//

#ifndef BLOCK_TYPE_H
#define BLOCK_TYPE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class BlockComponent;
    class BlockCondition;
    class Collision;
    class EventSystem;
    class InventoryLevel;
    class GameState;
    class Level;
    class Sprite;
    class SpriteGraphics;


// DEPENDENCIES
//===================================

    #include <map>
    #include "object.h"
    #include <vector>


// CLASS
//===================================

    class BlockType
    {
        public:
            friend class BlockComponentSwitch;

            BlockType
            (
                SpriteGraphics* g1 = nullptr,
                SpriteGraphics* g2 = nullptr,
                SpriteGraphics* g3 = nullptr,
                SpriteGraphics* g4 = nullptr,
                std::vector<BlockComponent*> components = {},
                std::vector<std::vector<BlockCondition*>> conditions = {}
            );

            ~BlockType();
            BlockType( const BlockType& ) = delete;
            BlockType( BlockType&& ) = delete;
            const BlockType& operator= ( const BlockType& ) = delete;
            const BlockType&& operator= ( BlockType&& ) = delete;

            void update( EventSystem& events );
            void interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
            void render( Graphics& graphics, Camera& camera, Block& block, bool priority );


        private:
            static const int BLOCK_SIZE = 16;
            static const int MINI_BLOCK_SIZE = 8;
            static const int NUM_O_MINI_BLOCKS = 4;

            std::shared_ptr<SpriteGraphics> graphics_[ NUM_O_MINI_BLOCKS ];
            std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions_;
            std::vector<std::unique_ptr<BlockComponent>> components_;
    };

#endif // BLOCK_TYPE_H
