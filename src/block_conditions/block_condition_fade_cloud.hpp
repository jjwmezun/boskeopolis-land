#pragma once

#include "block_condition.hpp"

class BlockConditionFadeCloud : public BlockCondition
{
	public:
		BlockConditionFadeCloud();
		~BlockConditionFadeCloud();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};