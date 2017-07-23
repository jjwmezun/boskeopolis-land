#include "block_condition_not_ducking.hpp"
#include "sprite.hpp"

BlockConditionNotDucking::BlockConditionNotDucking() {};

BlockConditionNotDucking::~BlockConditionNotDucking() {};

bool BlockConditionNotDucking::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return !sprite.isDucking();
};

