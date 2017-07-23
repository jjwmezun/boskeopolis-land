#include "block_condition_enemy.hpp"
#include "sprite.hpp"

BlockConditionEnemy::BlockConditionEnemy() {};

BlockConditionEnemy::~BlockConditionEnemy() {};

bool BlockConditionEnemy::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.hasType( Sprite::SpriteType::ENEMY );
};

