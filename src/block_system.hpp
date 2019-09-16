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
		void reset( const Map& lvmap );
		void blocksFromMap( Map& lvmap, const Camera& camera );
		bool blocksInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type = BlockComponent::Type::NOTYPE ) const;
		bool blocksInTheWayExcept( const sdl2::SDLRect& rect, BlockComponent::Type type ) const;
		BlockType* getBlockType( int type );
		const std::vector<Block>& getBlocksList() const;

	private:
		std::vector<Block> blocks_;
		std::map<std::string, Tileset> tilesets_;
		std::string current_tileset_;
		int map_width_;
		bool blocks_work_offscreen_;

		Tileset& getTileset();
		void addBlock( int x, int y, int i, int type );
		void changeTileset( std::string new_tileset );
};
