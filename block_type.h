


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

	#include "block_component.h"
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
                std::unique_ptr<SpriteGraphics> = nullptr,
                std::vector<std::unique_ptr<BlockComponent>> components = {},
                std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions = {}
            );

            ~BlockType();
            BlockType( const BlockType& ) = delete;
            BlockType( BlockType&& ) = default;
            const BlockType& operator= ( const BlockType& ) = delete;
            const BlockType&& operator= ( BlockType&& ) = delete;

            void update( EventSystem& events );
            void interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
            void render( Graphics& graphics, Camera& camera, Block& block, bool priority );
			
			bool hasComponentType( BlockComponent::Type type ) const;


        private:
            static constexpr int BLOCK_SIZE = 16;
            static constexpr int MINI_BLOCK_SIZE = 8;
            static constexpr int NUM_O_MINI_BLOCKS = 4;

			std::unique_ptr<SpriteGraphics> graphics_;
            std::vector<std::unique_ptr<BlockComponent>> components_;
            std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions_;
    };

#endif // BLOCK_TYPE_H
