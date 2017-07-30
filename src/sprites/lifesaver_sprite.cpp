#include "collision.hpp"
#include "lifesaver_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

LifesaverSprite::LifesaverSprite( int x, int y, Direction::Horizontal direction, bool start_moving )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/lifesaver.png" ), x, y, 48, 16, {}, 50, 2000, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, true, true, true )
{};

LifesaverSprite::~LifesaverSprite() {};

void LifesaverSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( in_water_ )
	{
		if ( hasMovementType( SpriteMovement::Type::GROUNDED ) )
		{
			changeMovement( SpriteMovement::Type::FLOATING );
			acceleration_y_ = 0;
		}
		else
		{
			moveUp();
		}
	}
	else
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
	}
	in_water_ = false;
};

void LifesaverSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideLeft() || their_collision.collideRight() )
		{
			//hit_box_.x += them.vx_ * 5;
		}

		if ( their_collision.collideTop() || their_collision.collideBottom() )
		{
			hit_box_.y += them.vy_ / 3;
		}

		if ( their_collision.collideAny() )
		{
			them.collideStopAny( their_collision );
			//them.changeY( hit_box_.y - them.heightSubPixels() );
		}
	}
};

void LifesaverSprite::reset()
{
	resetPosition();
	changeMovement( SpriteMovement::Type::GROUNDED );
	fullStopX();
	fullStopY();
	in_water_ = false;
};