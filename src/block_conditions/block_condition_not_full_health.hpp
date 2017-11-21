#pragma once

#include "block_condition.hpp"

class BlockConditionNotFullHealth : public BlockCondition
{
	public:
		BlockConditionNotFullHealth();
		~BlockConditionNotFullHealth();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};