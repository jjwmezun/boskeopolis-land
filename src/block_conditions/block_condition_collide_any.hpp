#ifndef BLOCK_CONDITION_COLLIDE_ANY_H
#define BLOCK_CONDITION_COLLIDE_ANY_H

#include "block_condition.hpp"

class BlockConditionCollideAny : public BlockCondition
{
	public:
		BlockConditionCollideAny();
		~BlockConditionCollideAny();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_ANY_H


