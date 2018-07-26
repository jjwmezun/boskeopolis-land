#pragma once

#include "block_condition.hpp"

class BlockConditionCollideRight : public BlockCondition
{
	public:
		BlockConditionCollideRight();
		~BlockConditionCollideRight();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};
