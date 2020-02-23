#pragma once

#include "block_condition.hpp"

class BlockConditionCollideTop : public BlockCondition
{
	public:
		BlockConditionCollideTop();
		~BlockConditionCollideTop();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
