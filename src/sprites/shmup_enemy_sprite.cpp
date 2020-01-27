#include "camera.hpp"
#include "collision.hpp"
#include "shmup_enemy_sprite.hpp"
#include "shmup_hero_bullet_sprite.hpp"
#include "main.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

#include <iostream>

static int getShootTargetTime()
{
	return mezun::randInt( 32, 8 );
};

ShmupEnemySprite::ShmupEnemySprite()
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 48, 16 ), -64, -64, 16, 16, { SpriteType::ENEMY }, 500, 4000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	reset_ ( true ),
	shoot_timer_ ( 0 ),
	shoot_timer_target_ ( getShootTargetTime() )
{};

ShmupEnemySprite::~ShmupEnemySprite() {};

void ShmupEnemySprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( reset_ || xPixels() < camera.x() )
	{
		doReset( camera );
	}
	else
	{
		moveLeft();
		if ( shoot_timer_ >= shoot_timer_target_ )
		{
			sprites.spawn( std::make_unique<ShmupHeroBulletSprite> ( xPixels(), centerYPixels(), Direction::Simple::LEFT, SpriteType::ENEMY ) );
			shoot_timer_ = 0;
			shoot_timer_target_ = getShootTargetTime();
		}
		else
		{
			++shoot_timer_;
		}
	}
};

void ShmupEnemySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
	{
		reset();
	}
};

void ShmupEnemySprite::reset()
{
	reset_ = true;
};

void ShmupEnemySprite::doReset( const Camera& camera )
{
	hit_box_.x = Unit::PixelsToSubPixels( camera.x() + camera.width() + 16 );
	hit_box_.y = Unit::PixelsToSubPixels( mezun::randInt( camera.y() + camera.height() - 16, camera.y() + 8 ) );
	vx_ = 0;
	reset_ = false;
	shoot_timer_ = 0;
};
