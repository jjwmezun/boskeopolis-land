#pragma once

#include "block_condition.hpp"

class BlockConditionCollideRight : public BlockCondition
{
	public:
		BlockConditionCollideRight();
		~BlockConditionCollideRight();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};
