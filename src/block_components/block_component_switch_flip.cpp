#include "block_component_switch_flip.hpp"
#include "event_system.hpp"

BlockComponentSwitchFlip::BlockComponentSwitchFlip() {};

BlockComponentSwitchFlip::~BlockComponentSwitchFlip() {};

void BlockComponentSwitchFlip::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	if ( ready_ )
	{
		events.flipSwitch();
		ready_ = false;
	}
};

void BlockComponentSwitchFlip::update( EventSystem& events, BlockType& type )
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
