#include "block_component_switch_on.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentSwitchOn::BlockComponentSwitchOn() {};
BlockComponentSwitchOn::~BlockComponentSwitchOn() {};

void BlockComponentSwitchOn::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().forceSwitchOn();
};
