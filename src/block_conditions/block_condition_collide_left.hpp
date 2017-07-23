#ifndef BLOCK_CONDITION_COLLIDE_LEFT_H
#define BLOCK_CONDITION_COLLIDE_LEFT_H

#include "block_condition.hpp"

class BlockConditionCollideLeft : public BlockCondition
{
	public:
		BlockConditionCollideLeft();
		~BlockConditionCollideLeft();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_LEFT_H
