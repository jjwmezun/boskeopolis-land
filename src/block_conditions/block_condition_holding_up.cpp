#include "block_condition_holding_up.h"
#include "sprite.h"

BlockConditionHoldingUp::BlockConditionHoldingUp() {};

BlockConditionHoldingUp::~BlockConditionHoldingUp() {};

bool BlockConditionHoldingUp::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.onGround();
};