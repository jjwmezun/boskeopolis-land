#ifndef BLOCK_CONDITION_COLLIDE_LEFT_H
#define BLOCK_CONDITION_COLLIDE_LEFT_H

#include "block_condition.hpp"

class BlockConditionCollideLeft : public BlockCondition
{
	public:
		BlockConditionCollideLeft();
		~BlockConditionCollideLeft();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_LEFT_H
