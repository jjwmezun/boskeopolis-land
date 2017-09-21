#pragma once

#include "block_condition.hpp"

class BlockConditionCollideAny : public BlockCondition
{
	public:
		BlockConditionCollideAny();
		~BlockConditionCollideAny();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};