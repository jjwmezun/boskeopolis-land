#pragma once

#include "block_condition.hpp"

class BlockConditionNotHero : public BlockCondition
{
	public:
		BlockConditionNotHero();
		~BlockConditionNotHero();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;
};
