#pragma once

#include "block_condition.hpp"

class BlockConditionNotFullHealth final : public BlockCondition
{
	public:
		BlockConditionNotFullHealth();
		~BlockConditionNotFullHealth();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};