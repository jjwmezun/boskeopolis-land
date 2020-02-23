#include "block.hpp"
#include "block_condition_collide_not_bottom.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideNotBottom::BlockConditionCollideNotBottom() {};

BlockConditionCollideNotBottom::~BlockConditionCollideNotBottom() {};

bool BlockConditionCollideNotBottom::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return collision.collideAny() && !sprite.collideBottomOnly( collision, block );
};
