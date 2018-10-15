#pragma once

#include "block_condition.hpp"

class BlockConditionNotOnLadder : public BlockCondition
{
	public:
		BlockConditionNotOnLadder();
		~BlockConditionNotOnLadder();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};
