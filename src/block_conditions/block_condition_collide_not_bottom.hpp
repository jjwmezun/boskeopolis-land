#pragma once

#include "block_condition.hpp"

class BlockConditionCollideNotBottom : public BlockCondition
{
	public:
		BlockConditionCollideNotBottom();
		~BlockConditionCollideNotBottom();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};
