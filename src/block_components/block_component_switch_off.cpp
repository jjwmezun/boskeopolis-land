#include "block_component_switch_off.hpp"
#include "event_system.hpp"

BlockComponentSwitchOff::BlockComponentSwitchOff() {};
BlockComponentSwitchOff::~BlockComponentSwitchOff() {};

void BlockComponentSwitchOff::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	events.forceSwitchOff();
};
