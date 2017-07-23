#include "block_condition_holding_up.hpp"
#include "sprite.hpp"

BlockConditionHoldingUp::BlockConditionHoldingUp() {};

BlockConditionHoldingUp::~BlockConditionHoldingUp() {};

bool BlockConditionHoldingUp::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.onGround();
};