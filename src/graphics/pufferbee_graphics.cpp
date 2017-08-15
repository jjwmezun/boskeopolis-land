#include "pufferbee_graphics.hpp"
#include "sprite.hpp"

PufferbeeGraphics::PufferbeeGraphics()
:
	SpriteGraphics( "sprites/pufferbee.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ),
	animation_frame_ ( { 0, 1, 0, true } ),
	animation_timer_ ()
{};

PufferbeeGraphics::~PufferbeeGraphics() {};

void PufferbeeGraphics::update( Sprite& sprite )
{
	if ( sprite.directionX() == Direction::Horizontal::RIGHT )
	{
		flip_x_ = true;
	}
	else
	{
		flip_x_ = false;
	}

	switch ( animation_frame_() )
	{
		case ( 1 ):
			current_frame_x_ = 24;
		break;
		case ( 0 ):
			current_frame_x_ = 0;
		break;
	}

	if ( animation_timer_.hit() )
	{
		++animation_frame_;
	}

	animation_timer_.update();
};