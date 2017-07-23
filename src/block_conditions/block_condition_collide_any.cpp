#include "block_condition_collide_any.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideAny::BlockConditionCollideAny() {};

BlockConditionCollideAny::~BlockConditionCollideAny() {};

bool BlockConditionCollideAny::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return collision.collideAny();
};
