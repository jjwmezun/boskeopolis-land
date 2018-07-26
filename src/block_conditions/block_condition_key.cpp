#include "block_condition_key.hpp"
#include "event_system.hpp"

BlockConditionKey::BlockConditionKey() {};
BlockConditionKey::~BlockConditionKey() {};

bool BlockConditionKey::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return events.hasKey();
};
