#ifndef BLOCK_CONDITION_COLLIDE_BOTTOM_H
#define BLOCK_CONDITION_COLLIDE_BOTTOM_H

#include "block_condition.hpp"

class BlockConditionCollideBottom : public BlockCondition
{
	public:
		BlockConditionCollideBottom();
		~BlockConditionCollideBottom();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_BOTTOM_H
