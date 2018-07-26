#pragma once

#include "block_condition.hpp"

class BlockConditionCollideLeft : public BlockCondition
{
	public:
		BlockConditionCollideLeft();
		~BlockConditionCollideLeft();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};
