#include "block_condition_not_full_health.hpp"
#include "level_state.hpp"

BlockConditionNotFullHealth::BlockConditionNotFullHealth() {};
BlockConditionNotFullHealth::~BlockConditionNotFullHealth() {};

bool BlockConditionNotFullHealth::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return level_state.health().hp() < level_state.health().maxHP();
};
