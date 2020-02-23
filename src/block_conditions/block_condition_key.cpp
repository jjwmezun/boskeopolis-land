#include "block_condition_key.hpp"
#include "event_system.hpp"

BlockConditionKey::BlockConditionKey() {};
BlockConditionKey::~BlockConditionKey() {};

bool BlockConditionKey::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return events.hasKey();
};
