#pragma once

#include "block_condition.hpp"

class BlockConditionCollideCenter : public BlockCondition
{
	public:
		BlockConditionCollideCenter();
		~BlockConditionCollideCenter();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};