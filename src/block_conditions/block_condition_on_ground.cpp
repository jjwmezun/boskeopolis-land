#include "block_condition_on_ground.hpp"
#include "sprite.hpp"

BlockConditionOnGround::BlockConditionOnGround() {};

BlockConditionOnGround::~BlockConditionOnGround() {};

bool BlockConditionOnGround::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return sprite.onGround();
};