#pragma once

#include "block_condition.hpp"

class BlockConditionNotDucking : public BlockCondition
{
	public:
		BlockConditionNotDucking();
		~BlockConditionNotDucking();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};
