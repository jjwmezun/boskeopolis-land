#pragma once

#include "block_condition.hpp"

class BlockConditionHoldingUp final : public BlockCondition
{
	public:
		BlockConditionHoldingUp();
		~BlockConditionHoldingUp();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};