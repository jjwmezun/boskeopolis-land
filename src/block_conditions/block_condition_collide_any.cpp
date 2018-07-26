#include "block_condition_collide_any.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideAny::BlockConditionCollideAny() {};

BlockConditionCollideAny::~BlockConditionCollideAny() {};

bool BlockConditionCollideAny::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return collision.collideAny();
};
