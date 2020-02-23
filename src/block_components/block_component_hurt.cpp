#include "block_component_hurt.hpp"
#include "health.hpp"

BlockComponentHurt::BlockComponentHurt() {};

BlockComponentHurt::~BlockComponentHurt() {};

void BlockComponentHurt::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	health.hurt();
};