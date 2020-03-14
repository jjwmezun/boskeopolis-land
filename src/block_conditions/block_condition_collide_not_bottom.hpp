#pragma once

#include "block_condition.hpp"

class BlockConditionCollideNotBottom final : public BlockCondition
{
	public:
		BlockConditionCollideNotBottom();
		~BlockConditionCollideNotBottom();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
