#include "block_condition_on_ground.hpp"
#include "sprite.hpp"

BlockConditionOnGround::BlockConditionOnGround() {};

BlockConditionOnGround::~BlockConditionOnGround() {};

bool BlockConditionOnGround::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.onGround();
};