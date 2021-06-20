#pragma once

#include "block_condition.hpp"

class BlockConditionBull final : public BlockCondition
{
	public:
		BlockConditionBull();
		~BlockConditionBull();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
