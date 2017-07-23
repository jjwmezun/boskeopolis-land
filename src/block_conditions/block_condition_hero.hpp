#ifndef BLOCK_CONDITION_HERO_H
#define BLOCK_CONDITION_HERO_H

#include "block_condition.hpp"

class BlockConditionHero : public BlockCondition
{
	public:
		BlockConditionHero();
		~BlockConditionHero();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_HERO_H
