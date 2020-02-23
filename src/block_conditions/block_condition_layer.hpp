#pragma once

#include "block_condition.hpp"

class BlockConditionLayer : public BlockCondition
{
	public:
		BlockConditionLayer( int layer );
		~BlockConditionLayer();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;

	private:
		int layer_;
};
