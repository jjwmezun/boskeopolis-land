#pragma once

class BlockType;
class Camera;
class EventSystem;
class Health;
class GameState;
class Level;
class Sprite;

#include "block.hpp"
#include "tileset.hpp"
#include "unit.hpp"
#include <unordered_map>
#include <vector>

class BlockSystem
{
	public:
		BlockSystem( const Level& level );

		void update( EventSystem& events );
		void render( const Camera& camera, bool priority = false );
		void interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health );

		void changedMap( const Map& lvmap );
		void blocksFromMap( const Map& lvmap, const Camera& camera );
		bool blocksInTheWay( const sdl::rect& r, BlockComponent::Type type ) const;
		void addBlock( int x, int y, int i, int type, std::vector<Block>& list );
		void clear();


	private:
		static constexpr int CAMERA_PADDING = 4;

		std::vector<Block> blocks_;
		std::vector<Tileset> tilesets_;
		std::unordered_map<int, int> tileset_ids_;
		int current_tileset_;

		Tileset& universalTileset();
		Tileset& mapTileset();
		int lastTilesetIndex() const;
		void setCurrentTileset( const Map& lvmap );
};