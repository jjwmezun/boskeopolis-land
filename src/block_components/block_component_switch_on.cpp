#include "block_component_switch_on.hpp"
#include "event_system.hpp"

BlockComponentSwitchOn::BlockComponentSwitchOn() {};
BlockComponentSwitchOn::~BlockComponentSwitchOn() {};

void BlockComponentSwitchOn::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.forceSwitchOn();
};
