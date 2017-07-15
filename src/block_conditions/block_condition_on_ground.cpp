#include "block_condition_on_ground.h"
#include "sprite.h"

BlockConditionOnGround::BlockConditionOnGround() {};

BlockConditionOnGround::~BlockConditionOnGround() {};

bool BlockConditionOnGround::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.onGround();
};