#pragma once

#include "block_condition.hpp"

class BlockConditionNotHero final : public BlockCondition
{
	public:
		BlockConditionNotHero();
		~BlockConditionNotHero();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};
