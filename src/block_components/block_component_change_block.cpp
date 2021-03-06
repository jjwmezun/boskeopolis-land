#include "block.hpp"
#include "block_component_change_block.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "map.hpp"

BlockComponentChangeBlock::BlockComponentChangeBlock( int new_block ) : new_block_ ( new_block ) {};

BlockComponentChangeBlock::~BlockComponentChangeBlock() {};

void BlockComponentChangeBlock::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.currentMap().changeBlock( block.layer(), block.location(), new_block_ );
};
