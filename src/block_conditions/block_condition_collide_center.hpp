#pragma once

#include "block_condition.hpp"

class BlockConditionCollideCenter final : public BlockCondition
{
	public:
		BlockConditionCollideCenter();
		~BlockConditionCollideCenter();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};