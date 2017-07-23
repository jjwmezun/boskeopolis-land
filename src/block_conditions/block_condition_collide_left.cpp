#include "block.hpp"
#include "block_condition_collide_left.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideLeft::BlockConditionCollideLeft() {};

BlockConditionCollideLeft::~BlockConditionCollideLeft() {};

bool BlockConditionCollideLeft::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return collision.collideLeft();
};

