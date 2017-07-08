#ifndef BLOCK_CONDITION_KEY_H
#define BLOCK_CONDITION_KEY_H

#include "block_condition.h"

class BlockConditionKey : public BlockCondition
{
	public:
		BlockConditionKey();
		~BlockConditionKey();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_KEY_H
