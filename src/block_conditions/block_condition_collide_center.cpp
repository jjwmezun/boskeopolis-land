#include "block.hpp"
#include "block_condition_collide_center.hpp"
#include "sprite.hpp"

BlockConditionCollideCenter::BlockConditionCollideCenter() {};

BlockConditionCollideCenter::~BlockConditionCollideCenter() {};

bool BlockConditionCollideCenter::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return sprite.centerXSubPixels() > block.xSubPixels()
        && sprite.centerXSubPixels() < block.rightSubPixels()
        && sprite.centerYSubPixels() > block.ySubPixels()
        && sprite.centerYSubPixels() < block.bottomSubPixels();
};
