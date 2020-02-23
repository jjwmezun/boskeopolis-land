#include "block_component_switch_off.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentSwitchOff::BlockComponentSwitchOff() {};
BlockComponentSwitchOff::~BlockComponentSwitchOff() {};

void BlockComponentSwitchOff::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().forceSwitchOff();
};
