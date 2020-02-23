#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "icicle_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "unit.hpp"

static constexpr int LEFT_EDGE = Unit::BlocksToSubPixels( 126 );

IcicleSprite::IcicleSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/icicle.png", 0, 0, false, false, 0.0, false, -2, -2, 4, 4 ), x, y + 2, 28, 8, { SpriteType::ICICLE }, 250, 1200, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, true, false )
{};

IcicleSprite::~IcicleSprite() {};

void IcicleSprite::customUpdate( LevelState& level_state )
{
	if ( hit_box_.x < LEFT_EDGE && !level_state.camera().onscreen( hit_box_ ) )
	{
		killNoAnimation();
	}
	else
	{
		moveLeft();
	}
};

void IcicleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideAny() )
		{
			them.collideStopAny( their_collision );
		}
	}
};