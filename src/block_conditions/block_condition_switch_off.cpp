#include "block_condition_switch_off.hpp"
#include "event_system.hpp"

BlockConditionSwitchOff::BlockConditionSwitchOff() {};
BlockConditionSwitchOff::~BlockConditionSwitchOff() {};

bool BlockConditionSwitchOff::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return !events.switch_;
};
