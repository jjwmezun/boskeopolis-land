
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
	#include "tileset.h"
    #include "unit.h"
    #include <vector>


// CLASS
//===================================

    class BlockSystem
    {
        public:
			BlockSystem( const Map& lvmap );
			
            void update( EventSystem& events );
            void render( Camera& camera, bool priority = false );
            void interact( Sprite& sprite, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
			
			void changeTileset( std::string new_tileset );
            void blocksFromMap( const Map& lvmap, const Camera& camera );
            bool blocksInTheWay( const sdl2::SDLRect& r, BlockComponent::Type type ) const;
			void addBlock( int x, int y, int i, int type, std::vector<Block>& list );


        private:
            static constexpr int CAMERA_PADDING = 4;
			
			std::vector<Block> blocks_;
			
			std::map<std::string, Tileset> tilesets_;
			std::string current_tileset_;

			Tileset& getTileset();
			
			int debug_num_ = 0;
    };

#endif // BLOCK_SYSTEM_H
