#include "pelican_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

static constexpr int NUMBER_OF_FRAMES = 9;
static constexpr int FRAME_WIDTH = 64;
static constexpr int FRAME_SPEED = 4;
static constexpr int DELAY_SPEED = 1;
static constexpr int DISTANCE_TO_COME_BACK = Unit::BlocksToSubPixels( 8 );
static constexpr int GOING_AWAY_DURATION = ( NUMBER_OF_FRAMES - 1 ) * FRAME_SPEED;
static constexpr int STAND_PADDING = GOING_AWAY_DURATION / 2;

PelicanSprite::PelicanSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pelican.png", 0, 0, false, false, 0, false, -7, -6, 16, 6 ), x, y, 48, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	state_ ( State::HERE ),
	timer_ ( 0 ),
	stepped_on_ ( false )
{};

PelicanSprite::~PelicanSprite() {};

void PelicanSprite::customUpdate( LevelState& level_state )
{
	switch ( state_ )
	{
		case ( State::HERE ):
		{
			if ( stepped_on_ )
			{
				++timer_;
			}
			else
			{
				timer_ = 0;
			}

			if ( timer_ >= DELAY_SPEED )
			{
				state_ = State::GOING_AWAY;
				timer_ = 0;
				stepped_on_ = false;
			}
		}
		break;

		case ( State::GOING_AWAY ):
		{
			const int frame = floor( timer_ / 4 );
			graphics_->current_frame_x_ = frame * FRAME_WIDTH;

			if ( timer_ >= GOING_AWAY_DURATION )
			{
				state_ = State::THERE;
				timer_ = 0;
			}
			else
			{
				++timer_;
			}
		}
		break;

		case ( State::COMING_BACK ):
		{
			const int frame = floor( timer_ / 4 );
			graphics_->current_frame_x_ = ( NUMBER_OF_FRAMES - 1 - frame ) * FRAME_WIDTH;

			if ( timer_ >= ( NUMBER_OF_FRAMES - 1 ) * FRAME_SPEED )
			{
				state_ = State::HERE;
				timer_ = 0;
			}
			else
			{
				++timer_;
			}
		}
		break;
	}
};

void PelicanSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	switch ( state_ )
	{
		case ( State::HERE ):
		{
			if( standOnMe( their_collision, them ) )
			{
				stepped_on_ = true;
			}
		}
		break;

		case ( State::GOING_AWAY ):
		{
			if ( timer_ < STAND_PADDING )
			{
				standOnMe( their_collision, them );
			}
		}
		break;

		case ( State::THERE ):
		{
			if ( them.hasType( SpriteType::HERO ) )
			{
				if
				(
					them.hit_box_.x > rightSubPixels() + DISTANCE_TO_COME_BACK ||
					them.rightSubPixels() < hit_box_.x - DISTANCE_TO_COME_BACK
				)
				{
					++timer_;
					if ( timer_ >= 30 )
					{
						state_ = State::COMING_BACK;
						timer_ = 0;
					}
				}
				else
				{
					timer_ = 0;
				}
			}
		}
		break;
	}
};

bool PelicanSprite::standOnMe( Collision& their_collision, Sprite& them )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideBottom() )
	{
		them.collideStopYBottom( their_collision.overlapYBottom() );
		return true;
	}
	return false;
}

void PelicanSprite::reset()
{
	graphics_->current_frame_x_ = 0;
	timer_ = 0;
	state_ = State::HERE;
	stepped_on_ = false;
};
