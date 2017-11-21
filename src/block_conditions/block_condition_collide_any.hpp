#pragma once

#include "block_condition.hpp"

class BlockConditionCollideAny : public BlockCondition
{
	public:
		BlockConditionCollideAny();
		~BlockConditionCollideAny();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};