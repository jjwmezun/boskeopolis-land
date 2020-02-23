#pragma once

#include "block_condition.hpp"

class BlockConditionCollideBottom : public BlockCondition
{
	public:
		BlockConditionCollideBottom();
		~BlockConditionCollideBottom();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
