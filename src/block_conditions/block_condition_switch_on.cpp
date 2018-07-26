#include "block_condition_switch_on.hpp"
#include "event_system.hpp"

BlockConditionSwitchOn::BlockConditionSwitchOn() {};
BlockConditionSwitchOn::~BlockConditionSwitchOn() {};

bool BlockConditionSwitchOn::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return events.switch_;
};
