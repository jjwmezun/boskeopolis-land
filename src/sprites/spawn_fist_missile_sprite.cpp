#include "fist_missile_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "spawn_fist_missile_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static int newTargetTime()
{
	return mezun::randInt( 120, 60 );
};

SpawnFistMissileSprite::SpawnFistMissileSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/handgun.png", 30, 0, false, false, 0.0 ), x, y, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	timer_ ( 0 ),
	target_time_ ( newTargetTime() )
{
	layer_ = Unit::Layer::SPRITES_2;
};

SpawnFistMissileSprite::~SpawnFistMissileSprite() {};

void SpawnFistMissileSprite::customUpdate( LevelState& level_state )
{
    ++timer_;
	if ( timer_ == target_time_ )
	{
        level_state.sprites().spawn( std::make_unique<FistMissileSprite> ( xPixels(), yPixels() ) );
		timer_ = 0;
		target_time_ = newTargetTime();
	}
};

void SpawnFistMissileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const Collision collision = them.testBlockCollision( *this );
	if ( them.hasType( SpriteType::HERO ) && collision.collideAny() )
	{
		them.collideStopAny( collision );
	}
};
