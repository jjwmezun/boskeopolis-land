#pragma once

#include "block_condition.hpp"

class BlockConditionNotDucking final : public BlockCondition
{
	public:
		BlockConditionNotDucking();
		~BlockConditionNotDucking();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
