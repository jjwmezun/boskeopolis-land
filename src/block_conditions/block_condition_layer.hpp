#pragma once

#include "block_condition.hpp"

class BlockConditionLayer : public BlockCondition
{
	public:
		BlockConditionLayer( int layer );
		~BlockConditionLayer();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;

	private:
		int layer_;
};
