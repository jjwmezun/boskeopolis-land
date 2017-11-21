#pragma once

#include "block_condition.hpp"

class BlockConditionSwitchOn : public BlockCondition
{
	public:
		BlockConditionSwitchOn();
		~BlockConditionSwitchOn();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};