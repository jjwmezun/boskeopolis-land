#include "block.hpp"
#include "block_condition_collide_top.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideTop::BlockConditionCollideTop() {};

BlockConditionCollideTop::~BlockConditionCollideTop() {};

bool BlockConditionCollideTop::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
{
	return sprite.collideTopOnly( collision, block );
};

