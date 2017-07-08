#include "block.h"
#include "block_condition_collide_bottom.h"
#include "collision.h"
#include "sprite.h"

BlockConditionCollideBottom::BlockConditionCollideBottom() {};

BlockConditionCollideBottom::~BlockConditionCollideBottom() {};

bool BlockConditionCollideBottom::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.collideBottomOnly( collision, block );
};

