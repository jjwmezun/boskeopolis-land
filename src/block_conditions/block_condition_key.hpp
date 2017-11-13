#pragma once

#include "block_condition.hpp"

class BlockConditionKey : public BlockCondition
{
	public:
		BlockConditionKey();
		~BlockConditionKey();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};