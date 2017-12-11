#include "block_component_switch.hpp"
#include "event_system.hpp"

BlockComponentSwitch::BlockComponentSwitch() {};

BlockComponentSwitch::~BlockComponentSwitch() {};

void BlockComponentSwitch::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	if ( ready_ )
	{
		events.flipSwitch();
		ready_ = false;
	}
};

void BlockComponentSwitch::update( EventSystem& events, BlockType& type )
{
	if ( !ready_ )
	{
		if ( delay_.done() )
		{
			ready_ = true;
			delay_.stop();
		}
		else if ( delay_.on() )
		{
			delay_.update();
		}
		else
		{
			delay_.start();
		}
	}
};
