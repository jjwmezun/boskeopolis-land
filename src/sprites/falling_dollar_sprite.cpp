#include "falling_dollar_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

FallingDollarSprite::FallingDollarSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/dollar.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true ),
	center_y_ ( ( double )( centerYPixels() ) ),
	center_x_ ( ( double )( centerXPixels() ) ),
	angle_ ( 0.5 )
{};

FallingDollarSprite::~FallingDollarSprite() {};

void FallingDollarSprite::customUpdate( LevelState& level_state )
{
	center_y_ += 1;
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
		{
			angle_ += 0.05;
			if ( angle_ >= 3.14 )
			{
				angle_ = 3.14;
				direction_x_ = Direction::Horizontal::RIGHT;
			}
		}
		break;
		case ( Direction::Horizontal::RIGHT ):
		{
			angle_ -= 0.05;
			if ( angle_ <= 0.0 )
			{
				angle_ = 0.0;
				direction_x_ = Direction::Horizontal::LEFT;
			}
		}
		break;
	}
	hit_box_.x = Unit::PixelsToSubPixels( center_x_ + ( std::cos( angle_ ) * 32.0 ) );
	hit_box_.y = Unit::PixelsToSubPixels( center_y_ + ( std::sin( angle_ ) * 32.0 ) );
};

void FallingDollarSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
