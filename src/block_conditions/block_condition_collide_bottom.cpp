#include "block.hpp"
#include "block_condition_collide_bottom.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideBottom::BlockConditionCollideBottom() {};

BlockConditionCollideBottom::~BlockConditionCollideBottom() {};

bool BlockConditionCollideBottom::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.collideBottomOnly( collision, block );
};

