#ifndef BLOCK_CONDITION_ENEMY_H
#define BLOCK_CONDITION_ENEMY_H

#include "block_condition.hpp"

class BlockConditionEnemy : public BlockCondition
{
	public:
		BlockConditionEnemy();
		~BlockConditionEnemy();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
};

#endif // BLOCK_CONDITION_ENEMY_H
