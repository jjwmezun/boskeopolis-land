#include "event_system.hpp"
#include "player_graphics.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"

PlayerGraphics::PlayerGraphics()
:
	animation_timer_ (),
	walk_counter_  (),
	climb_counter_ (),
	blink_counter_ (),
	swim_counter_  (),
	swim_timer_    ()
{};

PlayerGraphics::~PlayerGraphics() {};

void PlayerGraphics::update( const Sprite& sprite, SpriteGraphics* graphics, const EventSystem* events )
{
	if ( sprite.directionX() == Direction::Horizontal::RIGHT )
	{
		graphics->flip_x_ = true;
	}
	else
	{
		graphics->flip_x_ = false;
	}

	if ( sprite.on_ladder_ )
	{
		if ( sprite.isMoving() )
		{
			if ( animation_timer_.hit() )
			{
				++climb_counter_;
			}

			animation_timer_.update();
		}

		graphics->current_frame_x_ = ( climb_counter_.value() == 1 ) ? 128 : 96;
		graphics->current_frame_y_ = 0;
	}
	else if ( events != nullptr && events->is_sliding_prev_ )
	{
		graphics->current_frame_x_ = 48;
		graphics->current_frame_y_ = 26;
	}
	else if ( sprite.isDucking() )
	{
		if ( animation_timer_.hit() )
		{
			++blink_counter_;
		}

		animation_timer_.update();

		if ( blink_counter_ == 3 )
		{
			graphics->current_frame_x_ = 0;
			graphics->current_frame_y_ = 32;
		}
		else
		{
			graphics->current_frame_x_ = 64;
			graphics->current_frame_y_ = 6;
		}
	}
	else if ( !sprite.onGroundPrev() )
	{
		graphics->current_frame_x_ = 48;
		graphics->current_frame_y_ = 0;
	}
	else if ( sprite.isMoving() )
	{
		if ( animation_timer_.hit() )
		{
			++walk_counter_;
		}

		animation_timer_.update();

		switch ( walk_counter_.value() )
		{
			case 1:
				graphics->current_frame_x_ = 0;
			break;
			case 2:
				graphics->current_frame_x_ = 32;
			break;
			case 3:
				graphics->current_frame_x_ = 0;
			break;
			default:
				graphics->current_frame_x_ = 16;
			break;
		}

		graphics->current_frame_y_ = 0;
	}
	else
	{
		if ( animation_timer_.hit() )
		{
			++blink_counter_;
		}

		animation_timer_.update();

		switch ( blink_counter_.value() )
		{
			case 3:
				graphics->current_frame_x_ = 112;
				break;
			default:
				if ( sprite.lookingUp() )
					graphics->current_frame_x_ = 80;
				else
					graphics->current_frame_x_ = 0;
		}

		graphics->current_frame_y_ = 0;
	}

	if ( sprite.hasMovementType( SpriteMovement::Type::SWIMMING ) && !sprite.onGroundPrev() )
	{
		switch ( swim_counter_() )
		{
			case ( 0 ):
				graphics->current_frame_x_ = 64;
				graphics->current_frame_y_ = 26;
			break;

			case ( 1 ):
				graphics->current_frame_x_ = 80;
				graphics->current_frame_y_ = 26;
			break;

			case ( 2 ):
				graphics->current_frame_x_ = 96;
				graphics->current_frame_y_ = 26;
			break;
		}

		if ( swim_timer_.hit() )
		{
			++swim_counter_;

			if ( swim_counter_ != 0 )
			{
				swim_timer_.start();
			}
			else
			{
				swim_timer_.restart();
				swim_timer_.stop();
			}
		}

		if ( swim_timer_.on() )
		{
			swim_timer_.update();
		}

		if ( sprite.isJumpingPrev() )
		{
			if ( !swim_timer_.on() )
			{
				swim_timer_.start();
			}
		}
	}
	else
	{
		swim_timer_.restart();
		swim_timer_.stop();
		swim_counter_.reset();
	}

	if ( sprite.isDead() )
	{
		graphics->current_frame_x_ = 16;
		graphics->current_frame_y_ = 26;
	}
};
