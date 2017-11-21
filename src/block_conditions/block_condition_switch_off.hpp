#pragma once

#include "block_condition.hpp"

class BlockConditionSwitchOff : public BlockCondition
{
	public:
		BlockConditionSwitchOff();
		~BlockConditionSwitchOff();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};