#pragma once

#include "block_condition.hpp"

class BlockConditionOnGround : public BlockCondition
{
	public:
		BlockConditionOnGround();
		~BlockConditionOnGround();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};