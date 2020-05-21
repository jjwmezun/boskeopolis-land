#include "thrower_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "thrower_projectile_sprite.hpp"

static int generateTargetThrowTime()
{
	return mezun::randInt( 75, 45 );
};

ThrowerSprite::ThrowerSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/axe-monster.png", 12, 0, false, false, 0.0, false, -2, -1, 5, 1 ), x, y, 11, 23, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 0, 0, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	throw_timer_ ( 0 ),
	target_throw_time_ ( generateTargetThrowTime() )
{
	flipGraphicsOnRight();
};

ThrowerSprite::~ThrowerSprite() {};

void ThrowerSprite::customUpdate( LevelState& level_state )
{
	++throw_timer_;
	if ( throw_timer_ == target_throw_time_ )
	{
		target_throw_time_ = generateTargetThrowTime();
		throw_timer_ = 0;
		level_state.sprites().spawn( std::make_unique<ThrowerProjectileSprite> ( centerXPixels(), centerYPixels(), direction_x_ ) );
	}
};

void ThrowerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
