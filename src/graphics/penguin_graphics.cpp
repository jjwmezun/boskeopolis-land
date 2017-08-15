#include "penguin_graphics.hpp"
#include "sprite.hpp"

PenguinGraphics::PenguinGraphics()
:
	SpriteGraphics ( "sprites/penguin.png" ),
	animation_frame_ ( { 0, NUM_O_FRAMES-1, 0, true } ),
	animation_timer_ ()
{};

PenguinGraphics::~PenguinGraphics() {};

void PenguinGraphics::update( Sprite& sprite )
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
		case ( 0 ):
		case ( 2 ):
			current_frame_x_ = 0;
		break;
		case ( 1 ):
			current_frame_x_ = 16;
		break;
		case ( 3 ):
			current_frame_x_ = 32;
		break;
	}

	if ( animation_timer_.hit() )
	{
		++animation_frame_;
	}

	animation_timer_.update();

};

