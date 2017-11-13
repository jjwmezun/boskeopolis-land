#ifndef BLOCK_CONDITION_RIVAL_H
#define BLOCK_CONDITION_RIVAL_H

#include "block_condition.hpp"

class BlockConditionRival : public BlockCondition
{
	public:
		BlockConditionRival();
		~BlockConditionRival();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_RIVAL_H
