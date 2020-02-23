#include "block_condition_switch_off.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockConditionSwitchOff::BlockConditionSwitchOff() {};
BlockConditionSwitchOff::~BlockConditionSwitchOff() {};

bool BlockConditionSwitchOff::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return !level_state.events().isSwitchOn();
};
