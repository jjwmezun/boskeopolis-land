#ifndef BLOCK_CONDITION_COLLIDE_RIGHT_H
#define BLOCK_CONDITION_COLLIDE_RIGHT_H

#include "block_condition.hpp"

class BlockConditionCollideRight : public BlockCondition
{
	public:
		BlockConditionCollideRight();
		~BlockConditionCollideRight();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_RIGHT_H
