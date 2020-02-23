#pragma once

#include "block_condition.hpp"

class BlockConditionSwitchOff : public BlockCondition
{
	public:
		BlockConditionSwitchOff();
		~BlockConditionSwitchOff();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};