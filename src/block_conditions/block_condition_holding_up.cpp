#include "block_condition_holding_up.hpp"
#include "sprite.hpp"

BlockConditionHoldingUp::BlockConditionHoldingUp() {};

BlockConditionHoldingUp::~BlockConditionHoldingUp() {};

bool BlockConditionHoldingUp::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return sprite.onGround();
};