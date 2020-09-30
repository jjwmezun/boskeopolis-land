#include "firebar_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

static constexpr double SWING_SPEED = 0.1;
static constexpr int HIT_BOX_PADDING = 2000;

template<int n>
static constexpr CircleMovement generateCircle( int x, int y )
{
	const sdl2::SDLRect r = Unit::PixelsToSubPixels( sdl2::SDLRect{ x - 8, y - 8, 16, 16 } );
	return CircleMovement
	(
		r,
		r.x,
		r.y,
		Unit::BlocksToSubPixels( n ),
		SWING_SPEED
	);
}

FirebarSprite::FirebarSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/sword.png", 0, 0, false, false, 0, -32, -32, 64, 64 ), x, y, 16, 16, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	circles_
	({
		generateCircle<1>( x, y ),
		generateCircle<2>( x, y )
	})
{};

FirebarSprite::~FirebarSprite() {};

void FirebarSprite::customUpdate( LevelState& level_state )
{
	for ( auto& circle : circles_ )
	{
		circle.update();
	}
	const double angle = circles_[ 0 ].getAngle();
	const double angle_percent = angle / mezun::DOUBLE_PI;
	graphics_->rotation_ = 360 * angle_percent;
};

void FirebarSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		for ( const auto& circle : circles_ )
		{
			const auto& fireball = circle.getPosition();
			const bool touching =
				them.hit_box_.x < fireball.right() - HIT_BOX_PADDING &&
				them.rightSubPixels() > fireball.x + HIT_BOX_PADDING &&
				them.hit_box_.y < fireball.bottom() - HIT_BOX_PADDING &&
				them.bottomSubPixels() > fireball.y + HIT_BOX_PADDING;
			if ( touching )
			{
				level_state.health().hurt();
				return;
			}
		}
	}
};

void FirebarSprite::render( const Camera& camera ) const
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
};
