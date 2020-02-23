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
		void init( const Map& lvmap );
		void update( EventSystem& events );
		void render( const Map& lvmap, const Camera& camera, bool priority = false );
		void interact( Sprite& sprite, LevelState& level_state );
		void reset( const Map& lvmap );
		void blocksFromMap( LevelState& level_state );
		bool blocksInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type = BlockComponent::Type::NOTYPE ) const;
		bool blocksInTheWayExcept( const sdl2::SDLRect& rect, BlockComponent::Type type ) const;
		BlockType* getBlockType( int type );
		const std::vector<Block>& getBlocksList() const;
		const std::vector<const Block*> getSolidBlocksInField( const sdl2::SDLRect& rect, const Camera& camera, const Sprite& sprite, LevelState& level_state ) const;

	private:
		bool blocks_work_offscreen_;
		int map_width_;
		std::vector<Block> blocks_;
		std::string current_tileset_;
		std::map<std::string, Tileset> tilesets_;

		Tileset& getTileset();
		void addBlock( int x, int y, int i, int type );
		void changeTileset( std::string new_tileset );
};
