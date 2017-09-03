#include "collision.hpp"
#include "rolling_ball_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

RollingBallSprite::RollingBallSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/rolling_ball.png" ), x, y, 32, 32, {}, 100, 2000, 1000, 1000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

RollingBallSprite::~RollingBallSprite() {};

void RollingBallSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
};

void RollingBallSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
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
