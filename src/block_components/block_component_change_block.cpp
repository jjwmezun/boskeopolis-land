#include "block.hpp"
#include "block_component_change_block.hpp"
#include "level.hpp"
#include "map.hpp"

BlockComponentChangeBlock::BlockComponentChangeBlock( int new_block ) : new_block_ ( new_block ) {};

BlockComponentChangeBlock::~BlockComponentChangeBlock() {};

void BlockComponentChangeBlock::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	level.currentMap().changeBlock( block.location(), new_block_ );
};
