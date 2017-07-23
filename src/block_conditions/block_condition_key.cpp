#include "block_condition_key.hpp"
#include "event_system.hpp"

BlockConditionKey::BlockConditionKey() {};
BlockConditionKey::~BlockConditionKey() {};

bool BlockConditionKey::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return events.hasKey();
};
