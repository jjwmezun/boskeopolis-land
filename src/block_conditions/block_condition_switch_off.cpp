#include "block_condition_switch_off.hpp"
#include "event_system.hpp"

BlockConditionSwitchOff::BlockConditionSwitchOff() {};
BlockConditionSwitchOff::~BlockConditionSwitchOff() {};

bool BlockConditionSwitchOff::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return !events.isSwitchOn();
};
