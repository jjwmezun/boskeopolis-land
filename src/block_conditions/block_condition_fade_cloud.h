#pragma once

#include "block_condition.h"

class BlockConditionFadeCloud : public BlockCondition
{
	public:
		BlockConditionFadeCloud();
		~BlockConditionFadeCloud();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};