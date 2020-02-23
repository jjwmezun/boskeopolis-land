#include "block_condition_key.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockConditionKey::BlockConditionKey() {};
BlockConditionKey::~BlockConditionKey() {};

bool BlockConditionKey::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return level_state.events().hasKey();
};
