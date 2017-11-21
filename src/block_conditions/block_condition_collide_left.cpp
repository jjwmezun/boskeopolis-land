#include "block.hpp"
#include "block_condition_collide_left.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideLeft::BlockConditionCollideLeft() {};

BlockConditionCollideLeft::~BlockConditionCollideLeft() {};

bool BlockConditionCollideLeft::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
{
	return collision.collideLeft();
};

