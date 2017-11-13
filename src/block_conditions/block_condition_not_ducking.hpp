#ifndef BLOCK_CONDITION_NOT_DUCKING_H
#define BLOCK_CONDITION_NOT_DUCKING_H

#include "block_condition.hpp"

class BlockConditionNotDucking : public BlockCondition
{
	public:
		BlockConditionNotDucking();
		~BlockConditionNotDucking();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_NOT_DUCKING_H
