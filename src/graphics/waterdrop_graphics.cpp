#include "waterdrop_graphics.hpp"
#include "sprite.hpp"

WaterdropGraphics::WaterdropGraphics()
:
	SpriteGraphics ( "sprites/waterdrop.png", 5 * 14, 13, false, false, 0, false, 0, 0, 0, 0 ),
	init_ ( true ),
	animation_timer_ ()
{};

WaterdropGraphics::~WaterdropGraphics() {};

void WaterdropGraphics::update( Sprite& sprite )
{
	if ( !sprite.deathFinished() && sprite.isDead() )
	{
		if ( animation_timer_.hit() )
		{
			current_frame_x_ = std::min( current_frame_x_ + 14, 5 * 14 );
		}
		else if ( animation_timer_.on() )
		{
			animation_timer_.update();
		}
		else
		{
			animation_timer_.start();
		}
	}
	else if ( init_ )
	{
		if ( animation_timer_.hit() )
		{
			if ( current_frame_x_ <= 0 )
			{
				init_ = false;
			}
			else
			{
				current_frame_x_ = std::max( current_frame_x_ - 14, 0 );
				
				switch ( current_frame_x_ )
				{
					case ( 14 * 1 ):
						current_frame_y_ = 3;
					break;
					case ( 14 * 2 ):
						current_frame_y_ = 4;
					break;
					case ( 14 * 3 ):
						current_frame_y_ = 8;
					break;
					case ( 14 * 4 ):
						current_frame_y_ = 10;
					break;
					case ( 14 * 5 ):
						current_frame_y_ = 13;
					break;

					default:
						current_frame_y_ = 0;
					break;
				}
			}
		}
		else if ( animation_timer_.on() )
		{
			animation_timer_.update();
		}
		else
		{
			animation_timer_.start();
		}
	}
};