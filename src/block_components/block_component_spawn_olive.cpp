#include "block.hpp"
#include "block_component_spawn_olive.hpp"
#include "main.hpp"
#include "sprite_system.hpp"

BlockComponentSpawnOlive::BlockComponentSpawnOlive() {};

BlockComponentSpawnOlive::~BlockComponentSpawnOlive() {};

void BlockComponentSpawnOlive::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	if ( Main::stateFrame() % 135 == 0 )
	{
		sprites.spawnOlive( block.xPixels() + 4, block.yPixels() );
	}
};
