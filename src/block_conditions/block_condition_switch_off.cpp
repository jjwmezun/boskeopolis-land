#include "block_condition_switch_off.hpp"
#include "event_system.hpp"

BlockConditionSwitchOff::BlockConditionSwitchOff() {};
BlockConditionSwitchOff::~BlockConditionSwitchOff() {};

bool BlockConditionSwitchOff::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return !events.isSwitchOn();
};
