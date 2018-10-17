#pragma once

class BlockType;
class Camera;
class EventSystem;
class Health;
class GameState;
class Level;
class Sprite;
class SpriteSystem;

#include "block.hpp"
#include "tileset.hpp"
#include "unit.hpp"
#include <vector>

class BlockSystem
{
	public:
		BlockSystem( const Map& lvmap );
		void update( EventSystem& events );
		void render( const Map& lvmap, const Camera& camera, bool priority = false );
		void interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health, SpriteSystem& sprites );
		void changeTileset( std::string new_tileset );
		void blocksFromMap( const Map& lvmap, const Camera& camera );
		bool blocksInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type ) const;
		void addBlock( int x, int y, int i, int type, std::vector<Block>& list, bool accept_all_blocks = false );

	private:
		std::vector<Block> blocks_;
		std::map<std::string, Tileset> tilesets_;
		std::string current_tileset_;

		Tileset& getTileset();
};
