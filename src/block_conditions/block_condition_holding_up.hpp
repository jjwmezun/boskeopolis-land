#pragma once

#include "block_condition.hpp"

class BlockConditionHoldingUp : public BlockCondition
{
	public:
		BlockConditionHoldingUp();
		~BlockConditionHoldingUp();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};