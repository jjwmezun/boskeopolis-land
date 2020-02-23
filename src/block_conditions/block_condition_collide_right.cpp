#include "block.hpp"
#include "block_condition_collide_right.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockConditionCollideRight::BlockConditionCollideRight() {};

BlockConditionCollideRight::~BlockConditionCollideRight() {};

bool BlockConditionCollideRight::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return collision.collideRight();
};

