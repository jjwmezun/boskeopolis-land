#pragma once

#include "block_condition.hpp"

class BlockConditionOnGround : public BlockCondition
{
	public:
		BlockConditionOnGround();
		~BlockConditionOnGround();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};