#pragma once

class BlockType;
class Camera;
class EventSystem;
class Health;
class GameState;
class Level;
class LevelState;
class Sprite;
class SpriteSystem;

#include "block.hpp"
#include "tileset.hpp"
#include "unit.hpp"
#include <vector>

class BlockSystem final
{
	public:
		struct BlockLayer
		{
			std::vector<Block> blocks_;
			int renderable_id_;
		};
		void init( const Map& lvmap );
		void update( EventSystem& events );
		void renderLayer( const LevelState& level_state, int layer ) const;
		void renderLayerAllBlocks( const LevelState& level_state, int layer ) const;
		void renderTiles( const std::vector<int>& tiles, int width, int height ) const;
		void interact( Sprite& sprite, LevelState& level_state );
		void reset( LevelState& level_state );
		void blocksFromMap( LevelState& level_state );
		bool blocksInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type = BlockComponent::Type::NOTYPE ) const;
		bool blocksInTheWayWhetherCollided( const sdl2::SDLRect& rect, BlockComponent::Type type = BlockComponent::Type::NOTYPE ) const;
		bool blocksInTheWayGeneric( const sdl2::SDLRect& rect, BlockComponent::Type type = BlockComponent::Type::NOTYPE, bool whether_collided = false ) const;
		bool blocksInTheWayExcept( const sdl2::SDLRect& rect, BlockComponent::Type type ) const;
		const BlockType* getConstBlockType( int type ) const;
		BlockType* getBlockType( int type );
		const std::vector<Block>& getBlocksList() const;
		const std::vector<const Block*> getSolidBlocksInField( const sdl2::SDLRect& rect, const Camera& camera, const Sprite& sprite, LevelState& level_state ) const;

	private:
		bool blocks_work_offscreen_;
		int map_width_;
		std::vector<BlockLayer> layers_;
		std::string current_tileset_;
		std::map<std::string, Tileset> tilesets_;

		Tileset& getTileset();
		const Tileset& getConstTileset() const;
		void addBlock( int layer_index, int x, int y, int i, int type, LevelState& level_state );
		void changeTileset( std::string new_tileset );
};
