#include "shmup_shooter_sprite.hpp"
#include "collision.hpp"
#include "main.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

ShmupShooterSprite::ShmupShooterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 80 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true )
{};

ShmupShooterSprite::~ShmupShooterSprite() {};

void ShmupShooterSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) {};

void ShmupShooterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
	{
		kill();
	}
	else if ( them.hasType( SpriteType::HERO ) )
	{
		if ( Main::nextStateFrame( mezun::randInt( 60, 24 ) ) )
		{
			const double dx = ( double )( them.centerXSubPixels() - centerXSubPixels() );
			const double dy = ( double )( them.centerYSubPixels() - centerYSubPixels() );
			sprites.spawnShmupBullet( centerXPixels(), centerYPixels(), dy, dx );
		}
	}
};
