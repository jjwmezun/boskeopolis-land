#include "collision.hpp"
#include "lifesaver_sprite.hpp"
#include "sprite_graphics.hpp"

#include "main.hpp"
#include <iostream>

LifesaverSprite::LifesaverSprite( int x, int y, bool crate )
:
	Sprite( ( crate ) ? std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 192, 48 ) : std::make_unique<SpriteGraphics> ( "sprites/lifesaver.png" ), x, y, ( crate ) ? 32 : 48, ( crate ) ? 32 : 16, { SpriteType::BARREL }, 50, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, false, true, true, true ),
	spout_ ( nullptr ),
	can_push_down_ ( true )
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

	can_push_down_ = !collide_bottom_;
};

void LifesaverSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( their_collision.collideAny() )
	{
		if ( their_collision.collideBottom() )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			if ( can_push_down_ )
			{
				hit_box_.y += ( int )( them.vy_ / 3 );
			}
		}
		else
		{
			them.collideStopAny( their_collision );
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
