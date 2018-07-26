#pragma once

#include "block_condition.hpp"

class BlockConditionEnemy : public BlockCondition
{
	public:
		BlockConditionEnemy();
		~BlockConditionEnemy();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};
