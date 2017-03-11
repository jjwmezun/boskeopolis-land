#include "player_cart_graphics.h"
#include "sprite.h"

PlayerCartGraphics::PlayerCartGraphics()
:
	SpriteGraphics ( "sprites/autumn_cart.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 )
{};

PlayerCartGraphics::~PlayerCartGraphics() {};

void PlayerCartGraphics::update( Sprite& sprite )
{
	if ( sprite.isDucking() )
	{
		current_frame_x_ = 48;
		current_frame_y_ = 8;
	}
	else
	{
		switch ( sprite.directionX() )
		{
			case ( Direction::Horizontal::LEFT ):
				current_frame_x_ = 96;
				current_frame_y_ = 0;
			break;
				
			case ( Direction::Horizontal::RIGHT ):
				current_frame_x_ = 0;
				current_frame_y_ = 0;
			break;
		}
	}
};