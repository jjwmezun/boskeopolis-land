#ifndef BLOCK_CONDITION_COLLIDE_RIGHT_H
#define BLOCK_CONDITION_COLLIDE_RIGHT_H

#include "block_condition.h"

class BlockConditionCollideRight : public BlockCondition
{
	public:
		BlockConditionCollideRight();
		~BlockConditionCollideRight();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_RIGHT_H
