#ifndef BLOCK_CONDITION_HERO_H
#define BLOCK_CONDITION_HERO_H

#include "block_condition.hpp"

class BlockConditionHero : public BlockCondition
{
	public:
		BlockConditionHero();
		~BlockConditionHero();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_HERO_H
