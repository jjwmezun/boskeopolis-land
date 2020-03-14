#pragma once

#include "block_condition.hpp"

class BlockConditionKey final : public BlockCondition
{
	public:
		BlockConditionKey();
		~BlockConditionKey();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
};