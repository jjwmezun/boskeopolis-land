#include "block_component_kill.hpp"
#include "sprite.hpp"

BlockComponentKill::BlockComponentKill() {};

BlockComponentKill::~BlockComponentKill() {};

void BlockComponentKill::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	sprite.killNoAnimation();
};