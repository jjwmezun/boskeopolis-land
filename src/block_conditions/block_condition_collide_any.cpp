#include "block_condition_collide_any.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideAny::BlockConditionCollideAny() {};

BlockConditionCollideAny::~BlockConditionCollideAny() {};

bool BlockConditionCollideAny::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return collision.collideAny();
};
