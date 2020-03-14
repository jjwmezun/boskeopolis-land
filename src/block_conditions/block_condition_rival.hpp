#pragma once

#include "block_condition.hpp"

class BlockConditionRival final : public BlockCondition
{
	public:
		BlockConditionRival();
		~BlockConditionRival();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
