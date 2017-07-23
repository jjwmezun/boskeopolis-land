#ifndef BLOCK_CONDITION_NOT_DUCKING_H
#define BLOCK_CONDITION_NOT_DUCKING_H

#include "block_condition.hpp"

class BlockConditionNotDucking : public BlockCondition
{
	public:
		BlockConditionNotDucking();
		~BlockConditionNotDucking();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_NOT_DUCKING_H
