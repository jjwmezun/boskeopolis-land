#include "block_condition_key.h"
#include "event_system.h"

BlockConditionKey::BlockConditionKey() {};
BlockConditionKey::~BlockConditionKey() {};

bool BlockConditionKey::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return events.hasKey();
};
