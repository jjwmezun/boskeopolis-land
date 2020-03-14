#pragma once

#include "block_condition.hpp"

class BlockConditionCollideAny final : public BlockCondition
{
	public:
		BlockConditionCollideAny();
		~BlockConditionCollideAny();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};