#include "block_component_conveyor.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "sprite.hpp"

BlockComponentConveyor::BlockComponentConveyor( Direction::Horizontal direction, int speed )
:
	x_effect_ ( ( direction == Direction::Horizontal::LEFT ) ? -speed : speed )
{};

BlockComponentConveyor::~BlockComponentConveyor() {};

void BlockComponentConveyor::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	if ( ( x_effect_ < 0 && !sprite.collide_left_ ) || ( x_effect_ > 0 && !sprite.collide_right_ ) )
	{
		if ( events.switchOn() )
		{
			sprite.addToX( -x_effect_ );
		}
		else
		{
			sprite.addToX( x_effect_ );
		}
	}

	if ( collision.collideBottom() )
	{
		events.on_conveyor_belt_ = true;
	}
};