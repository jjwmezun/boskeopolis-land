#include "bomb_sprite.hpp"
#include <cmath>
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int pointDistance( Point a, Point b )
{
	return std::sqrt( std::pow( std::abs( a.x - b.x ), 2 ) + std::pow( std::abs( a.y - b.y ), 2 ) );
};

BombSprite::BombSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bomb.png" ), x, y, 16, 16, {}, 500, 2000, 1000, 6000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::DESPAWN_OFFSCREEN, true, false ),
	detonated_ ( false )
{
    on_ground_ = true;
};

BombSprite::~BombSprite() {};

void BombSprite::customUpdate( LevelState& level_state )
{
	if ( detonated_ )
	{
		const int center_x = centerXSubPixels();
		const int center_y = centerYSubPixels();
		hit_box_.w += Unit::PixelsToSubPixels( 2 );
		hit_box_.h += Unit::PixelsToSubPixels( 2 );
		if ( hit_box_.w >= Unit::PixelsToSubPixels( 48 ) )
		{
			hit_box_.w = Unit::PixelsToSubPixels( 48 );
			hit_box_.h = Unit::PixelsToSubPixels( 48 );
		}
		hit_box_.x = center_x - ( hit_box_.w / 2.0 );
		hit_box_.y = center_y - ( hit_box_.h / 2.0 );
	}
	else
	{
		jump();
		graphics_->rotation_ -= 5.0;
		moveLeft();
		if ( hit_box_.x < original_hit_box_.x - Unit::BlocksToSubPixels( 4 ) )
		{
			fullStopX();
			hit_box_.x = original_hit_box_.x - Unit::BlocksToSubPixels( 4 );
		}
	}
};

void BombSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( detonated_ )
		{
			if ( hit_box_.w > 4000 )
			{
				if
				(
					pointDistance( { centerXSubPixels(), centerYSubPixels() }, { them.rightSubPixels(), them.ySubPixels() } ) < halfWidthSubPixels() ||
					pointDistance( { centerXSubPixels(), centerYSubPixels() }, { them.xSubPixels(), them.ySubPixels() } ) < halfWidthSubPixels() ||
					pointDistance( { centerXSubPixels(), centerYSubPixels() }, { them.rightSubPixels(), them.bottomSubPixels() } ) < halfWidthSubPixels() ||
					pointDistance( { centerXSubPixels(), centerYSubPixels() }, { them.xSubPixels(), them.bottomSubPixels() } ) < halfWidthSubPixels()
				)
				{
					level_state.health().hurt();
				}
			}
		}
		else
		{
			if ( centerXSubPixels() < them.centerXSubPixels() + 32000 && centerXSubPixels() > them.centerXSubPixels() - 32000 && centerYSubPixels() < them.centerYSubPixels() + 32000 && centerYSubPixels() > them.centerYSubPixels() - 32000 )
			{
				detonated_ = true;
				fullStopX();
				hit_box_.w = Unit::PixelsToSubPixels( 2 );
				hit_box_.h = Unit::PixelsToSubPixels( 2 );
				hit_box_.x = centerXSubPixels() - Unit::PixelsToSubPixels( 1 );
				hit_box_.y = centerYSubPixels() - Unit::PixelsToSubPixels( 1 );
				graphics_->current_frame_x_ = 16;
				changeMovement( SpriteMovement::Type::FLOATING );
				fullStopY();
				graphics_->rotation_ = 0.0;
			}
		}
	}
};
