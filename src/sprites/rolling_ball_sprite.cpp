#include "collision.hpp"
#include "rolling_ball_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

RollingBallSprite::RollingBallSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/rolling_ball.png" ), x, y, 32, 32, {}, 100, 2000, 1000, 1000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

RollingBallSprite::~RollingBallSprite() {};

void RollingBallSprite::customUpdate( LevelState& level_state )
{
};

void RollingBallSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		acceleration_x_ = 0;
		if ( their_collision.collideBottom() )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			
			if ( them.centerXSubPixels() > centerXSubPixels() + 4000 )
			{
				vx_ += 1000;
			}
			else if ( them.centerXSubPixels() < centerXSubPixels() - 4000 )
			{
				vx_ -= 1000;
			}
			them.addToX( vx_ );
		}
	}
};
