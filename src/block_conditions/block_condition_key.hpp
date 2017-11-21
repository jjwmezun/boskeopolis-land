#pragma once

#include "block_condition.hpp"

class BlockConditionKey : public BlockCondition
{
	public:
		BlockConditionKey();
		~BlockConditionKey();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};