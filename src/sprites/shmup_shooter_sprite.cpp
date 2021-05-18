#include "shmup_shooter_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include <memory>
#include "mezun_math.hpp"
#include "shmup_bullet_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static int generateTargetTime()
{
	return mezun::randInt( 120, 60 );
};

ShmupShooterSprite::ShmupShooterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 80 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true ),
	timer_ ( 0 ),
	target_time_ ( generateTargetTime() )
{};

ShmupShooterSprite::~ShmupShooterSprite() {};

void ShmupShooterSprite::customUpdate( LevelState& level_state ) {};

void ShmupShooterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
	{
		kill();
	}
	else if ( them.hasType( SpriteType::HERO ) )
	{
		if ( timer_ == target_time_ )
		{
			const double dx = ( double )( them.centerXSubPixels() - centerXSubPixels() );
			const double dy = ( double )( them.centerYSubPixels() - centerYSubPixels() );
			level_state.sprites().spawn( std::unique_ptr<ShmupBulletSprite> ( new ShmupBulletSprite( centerXPixels(), centerYPixels(), dy, dx ) ) );
			timer_ = 0;
			target_time_ = generateTargetTime();
		}
		else
		{
			++timer_;
		}
	}
};
