#include "block_component_switch_on.hpp"
#include "event_system.hpp"

BlockComponentSwitchOn::BlockComponentSwitchOn() {};
BlockComponentSwitchOn::~BlockComponentSwitchOn() {};

void BlockComponentSwitchOn::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	events.switch_ = true;
};
