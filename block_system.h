
// Name
//===================================
//
// BlockSystem
//


#ifndef BLOCK_SYSTEM_H
#define BLOCK_SYSTEM_H


// FORWARD DECLARATIONS
//===================================

    class BlockType;
    class Camera;
    class EventSystem;
    class GameState;
    class InventoryLevel;
    class Level;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "unit.h"
    #include <vector>


// CLASS
//===================================

    class BlockSystem
    {
        public:
            BlockSystem();
            ~BlockSystem();

            void update( EventSystem& events );
            void render( Graphics& graphics, Camera& camera, bool priority = false );
            void interact( Sprite& sprite, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
            void blocksFromMap( Map& lvmap, Camera& camera );
            bool blocksInTheWay( const sdl2::SDLRect& r, const std::vector<int>& type_ids = {} ) const;
            bool blocksInTheWay( const sdl2::SDLRect& r, BlockComponent::Type type ) const;


        private:
            static constexpr int CAMERA_PADDING = 4;
			std::vector<Block> blocks_;
            std::vector<BlockType*> block_types_;

			void addBlock( int x, int y, int i, int type );
            BlockType* blockType( int n ) const;
            void clearBlockTypes();
    };

#endif // BLOCK_SYSTEM_H
