#ifndef BLOCK_CONDITION_COLLIDE_TOP_H
#define BLOCK_CONDITION_COLLIDE_TOP_H

#include "block_condition.hpp"

class BlockConditionCollideTop : public BlockCondition
{
	public:
		BlockConditionCollideTop();
		~BlockConditionCollideTop();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};

#endif // BLOCK_CONDITION_COLLIDE_TOP_H
