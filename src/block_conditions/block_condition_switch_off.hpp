#pragma once

#include "block_condition.hpp"

class BlockConditionSwitchOff : public BlockCondition
{
	public:
		BlockConditionSwitchOff();
		~BlockConditionSwitchOff();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};