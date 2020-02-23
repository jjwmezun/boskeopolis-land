#pragma once

#include "block_condition.hpp"

class BlockConditionEnemy : public BlockCondition
{
	public:
		BlockConditionEnemy();
		~BlockConditionEnemy();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};
