#pragma once

#include "block_condition.hpp"

class BlockConditionNotHero : public BlockCondition
{
	public:
		BlockConditionNotHero();
		~BlockConditionNotHero();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const override;
};
