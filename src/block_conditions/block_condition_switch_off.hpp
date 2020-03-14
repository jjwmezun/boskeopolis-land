#pragma once

#include "block_condition.hpp"

class BlockConditionSwitchOff final : public BlockCondition
{
	public:
		BlockConditionSwitchOff();
		~BlockConditionSwitchOff();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};