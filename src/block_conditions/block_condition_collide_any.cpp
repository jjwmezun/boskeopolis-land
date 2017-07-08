#include "block_condition_collide_any.h"
#include "collision.h"
#include "sprite.h"

BlockConditionCollideAny::BlockConditionCollideAny() {};

BlockConditionCollideAny::~BlockConditionCollideAny() {};

bool BlockConditionCollideAny::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return collision.collideAny();
};
