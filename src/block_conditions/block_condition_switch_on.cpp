#include "block_condition_switch_on.hpp"
#include "event_system.hpp"

BlockConditionSwitchOn::BlockConditionSwitchOn() {};
BlockConditionSwitchOn::~BlockConditionSwitchOn() {};

bool BlockConditionSwitchOn::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return events.isSwitchOn();
};
