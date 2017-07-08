#ifndef BLOCK_CONDITION_RIVAL_H
#define BLOCK_CONDITION_RIVAL_H

#include "block_condition.h"

class BlockConditionRival : public BlockCondition
{
	public:
		BlockConditionRival();
		~BlockConditionRival();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_RIVAL_H
