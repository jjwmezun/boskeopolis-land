#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "icicle_sprite.hpp"
#include "sprite_graphics.hpp"
#include "unit.hpp"
#include <iostream>

static constexpr int LEFT_EDGE = Unit::BlocksToSubPixels( 126 );

IcicleSprite::IcicleSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/icicle.png", 0, 0, false, false, 0.0, false, -2, -2, 4, 4 ), x, y + 2, 28, 8, { SpriteType::ICICLE }, 250, 1500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, true, false )
{};

IcicleSprite::~IcicleSprite() {};

void IcicleSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( hit_box_.x < LEFT_EDGE && !camera.onscreen( hit_box_ ) )
	{
		killNoAnimation();
	}
	else
	{
		moveLeft();
	}
};

void IcicleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		/*if ( them.collideBottomOnly( their_collision, *this ) )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
		}*/
		if ( their_collision.collideAny() )
		{
			//health.hurt();
			them.collideStopAny( their_collision );
		}
	}
};