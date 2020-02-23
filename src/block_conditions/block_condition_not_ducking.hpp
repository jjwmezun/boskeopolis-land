#pragma once

#include "block_condition.hpp"

class BlockConditionNotDucking : public BlockCondition
{
	public:
		BlockConditionNotDucking();
		~BlockConditionNotDucking();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};
