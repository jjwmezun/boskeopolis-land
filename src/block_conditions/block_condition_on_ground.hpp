#pragma once

#include "block_condition.hpp"

class BlockConditionOnGround : public BlockCondition
{
	public:
		BlockConditionOnGround();
		~BlockConditionOnGround();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};