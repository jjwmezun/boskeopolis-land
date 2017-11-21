#pragma once

#include "block_condition.hpp"

class BlockConditionHoldingUp : public BlockCondition
{
	public:
		BlockConditionHoldingUp();
		~BlockConditionHoldingUp();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};