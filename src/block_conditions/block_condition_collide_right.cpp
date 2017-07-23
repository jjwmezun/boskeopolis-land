#include "block.hpp"
#include "block_condition_collide_right.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideRight::BlockConditionCollideRight() {};

BlockConditionCollideRight::~BlockConditionCollideRight() {};

bool BlockConditionCollideRight::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return collision.collideRight();
};

