#include "block.hpp"
#include "block_condition_collide_bottom.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideBottom::BlockConditionCollideBottom() {};

BlockConditionCollideBottom::~BlockConditionCollideBottom() {};

bool BlockConditionCollideBottom::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
{
	return sprite.collideBottomOnly( collision, block );
};

