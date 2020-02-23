#include "block_component_kill_animated.hpp"
#include "sprite.hpp"

BlockComponentKillAnimated::BlockComponentKillAnimated() {};

BlockComponentKillAnimated::~BlockComponentKillAnimated() {};

void BlockComponentKillAnimated::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	sprite.kill();
};