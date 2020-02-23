#pragma once

#include "block_condition.hpp"

class BlockConditionRival : public BlockCondition
{
	public:
		BlockConditionRival();
		~BlockConditionRival();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};
