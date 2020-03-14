#pragma once

#include "block_condition.hpp"

class BlockConditionNotOnLadder final : public BlockCondition
{
	public:
		BlockConditionNotOnLadder();
		~BlockConditionNotOnLadder();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
