#include "block_condition_not_ducking.hpp"
#include "sprite.hpp"

BlockConditionNotDucking::BlockConditionNotDucking() {};

BlockConditionNotDucking::~BlockConditionNotDucking() {};

bool BlockConditionNotDucking::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events )
{
	return !sprite.isDucking();
};

