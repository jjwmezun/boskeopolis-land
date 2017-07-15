#pragma once

#include "block_condition.h"

class BlockConditionHoldingUp : public BlockCondition
{
	public:
		BlockConditionHoldingUp();
		~BlockConditionHoldingUp();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};