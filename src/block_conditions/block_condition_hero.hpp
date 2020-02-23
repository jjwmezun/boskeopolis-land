#pragma once

#include "block_condition.hpp"

class BlockConditionHero : public BlockCondition
{
	public:
		BlockConditionHero();
		~BlockConditionHero();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
