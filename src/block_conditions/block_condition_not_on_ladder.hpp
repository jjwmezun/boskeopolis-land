#pragma once

#include "block_condition.hpp"

class BlockConditionNotOnLadder : public BlockCondition
{
	public:
		BlockConditionNotOnLadder();
		~BlockConditionNotOnLadder();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};
