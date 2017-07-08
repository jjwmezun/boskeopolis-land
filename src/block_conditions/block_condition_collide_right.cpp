#include "block.h"
#include "block_condition_collide_right.h"
#include "collision.h"
#include "sprite.h"

BlockConditionCollideRight::BlockConditionCollideRight() {};

BlockConditionCollideRight::~BlockConditionCollideRight() {};

bool BlockConditionCollideRight::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return collision.collideRight();
};
