#ifndef BLOCK_CONDITION_COLLIDE_BOTTOM_H
#define BLOCK_CONDITION_COLLIDE_BOTTOM_H

#include "block_condition.hpp"

class BlockConditionCollideBottom : public BlockCondition
{
	public:
		BlockConditionCollideBottom();
		~BlockConditionCollideBottom();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_BOTTOM_H
