#pragma once

#include "block_condition.hpp"

class BlockConditionFadeCloud final : public BlockCondition
{
	public:
		BlockConditionFadeCloud();
		~BlockConditionFadeCloud();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};