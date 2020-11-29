#include "block_condition_switch.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockConditionSwitch::BlockConditionSwitch( int target ) : target_ ( target ) {};
BlockConditionSwitch::~BlockConditionSwitch() {};

bool BlockConditionSwitch::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return level_state.events().isSwitch( target_ );
};
