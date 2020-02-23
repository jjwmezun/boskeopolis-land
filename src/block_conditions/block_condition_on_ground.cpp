#include "block_condition_on_ground.hpp"
#include "sprite.hpp"

BlockConditionOnGround::BlockConditionOnGround() {};

BlockConditionOnGround::~BlockConditionOnGround() {};

bool BlockConditionOnGround::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return sprite.onGround();
};