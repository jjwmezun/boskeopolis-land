#pragma once

#include "block_condition.hpp"

class BlockConditionOnGround final : public BlockCondition
{
	public:
		BlockConditionOnGround();
		~BlockConditionOnGround();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};