#ifndef BLOCK_CONDITION_COLLIDE_TOP_H
#define BLOCK_CONDITION_COLLIDE_TOP_H

#include "block_condition.h"

class BlockConditionCollideTop : public BlockCondition
{
	public:
		BlockConditionCollideTop();
		~BlockConditionCollideTop();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_TOP_H