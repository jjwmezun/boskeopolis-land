#include "block_condition_not_on_ladder.hpp"
#include "sprite.hpp"

BlockConditionNotOnLadder::BlockConditionNotOnLadder() {};

BlockConditionNotOnLadder::~BlockConditionNotOnLadder() {};

bool BlockConditionNotOnLadder::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return !sprite.on_ladder_;
};