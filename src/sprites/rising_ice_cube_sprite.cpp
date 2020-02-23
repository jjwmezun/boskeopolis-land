#include "rising_ice_cube_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

static constexpr int MAX_SPEED = 3000;

RisingIceCubeSprite::RisingIceCubeSprite( int x, int y, int height_limit )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/icecube.png" ), x, y, 32, 32, {}, 100, 3000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	state_ ( State::RISING ),
	height_limit_ ( Unit::BlocksToSubPixels( height_limit ) )
{
	vy_ = 0;
};

RisingIceCubeSprite::~RisingIceCubeSprite() {};

void RisingIceCubeSprite::customUpdate( LevelState& level_state )
{
	switch ( state_ )
	{
		case ( State::PAUSED ):
			if ( collide_top_prev_ )
			{
				fullStopY();
			}
			else
			{
				state_ = State::FALLING;
			}
		break;

		case ( State::RISING ):
			if ( hit_box_.y <= original_hit_box_.y - height_limit_ )
			{
				fullStopY();
				state_ = State::FALLING;
			}
			else
			{
				moveUp();
			}
		break;
		
		case ( State::FALLING ):
			if ( collide_top_prev_ )
			{
			}
			if ( hit_box_.y >= original_hit_box_.y )
			{
				fullStopY();
				hit_box_.y = original_hit_box_.y;
				state_ = State::RISING;
			}
			else if ( vy_ == 0 )
			{
				moveDown();
			}
			else
			{
				//vy_ /= traction_;
			}
		break;
	}
};

void RisingIceCubeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideAny() && !their_collision.collideTop() )
		{	
			them.collideStopAny( their_collision );
		}

		if ( their_collision.collideBottom() )
		{
			them.hit_box_.y += vy_;
		}
		else if ( their_collision.collideTop() )
		{
			collideStopAny( my_collision );
			//state_ = State::RISING;

			if ( them.vy_ < 0 )
			{
				them.collideStopAny( their_collision );
			}
			state_ = State::RISING;
		}
	}
};