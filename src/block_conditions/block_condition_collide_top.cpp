#include "block.hpp"
#include "block_condition_collide_top.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideTop::BlockConditionCollideTop() {};

BlockConditionCollideTop::~BlockConditionCollideTop() {};

bool BlockConditionCollideTop::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.collideTopOnly( collision, block );
};

