#ifndef BLOCK_CONDITION_ENEMY_H
#define BLOCK_CONDITION_ENEMY_H

#include "block_condition.hpp"

class BlockConditionEnemy : public BlockCondition
{
	public:
		BlockConditionEnemy();
		~BlockConditionEnemy();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;
};

#endif // BLOCK_CONDITION_ENEMY_H
