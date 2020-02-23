#include "block_condition_switch_on.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockConditionSwitchOn::BlockConditionSwitchOn() {};
BlockConditionSwitchOn::~BlockConditionSwitchOn() {};

bool BlockConditionSwitchOn::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return level_state.events().isSwitchOn();
};
