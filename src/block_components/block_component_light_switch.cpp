#include "block_component_light_switch.hpp"
#include "event_system.hpp"

BlockComponentLightSwitch::BlockComponentLightSwitch() {};

BlockComponentLightSwitch::~BlockComponentLightSwitch() {};

void BlockComponentLightSwitch::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	events.lightSwitchTurnOn();
};