#include "block_component_switch_off.hpp"
#include "event_system.hpp"

BlockComponentSwitchOff::BlockComponentSwitchOff() {};
BlockComponentSwitchOff::~BlockComponentSwitchOff() {};

void BlockComponentSwitchOff::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.forceSwitchOff();
};
