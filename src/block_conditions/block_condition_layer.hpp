#pragma once

#include "block_condition.hpp"
#include "unit.hpp"

class BlockConditionLayer final : public BlockCondition
{
	public:
		BlockConditionLayer( Unit::Layer layer );
		~BlockConditionLayer();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;

	private:
		Unit::Layer layer_;
};
