#include "block_condition_enemy.hpp"
#include "sprite.hpp"

BlockConditionEnemy::BlockConditionEnemy() {};

BlockConditionEnemy::~BlockConditionEnemy() {};

bool BlockConditionEnemy::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return sprite.hasType( Sprite::SpriteType::ENEMY );
};

