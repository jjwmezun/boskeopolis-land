#include "anchor_missile_sprite.hpp"
#include "collision.hpp"
#include "direction.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "spawn_anchor_missile_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static int newTargetTime()
{
	return mezun::randInt( 90, 30 );
};

SpawnAnchorMissileSprite::SpawnAnchorMissileSprite( int x, int y, SpawnAnchorMissileSprite::AMType type )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/anchor-missile.png", 16 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	type_ ( type ),
	timer_ ( 0 ),
	target_time_ ( newTargetTime() )
{
	layer_ = Unit::Layer::SPRITES_2;
};

SpawnAnchorMissileSprite::~SpawnAnchorMissileSprite() {};

void SpawnAnchorMissileSprite::customUpdate( LevelState& level_state )
{
	SpriteSystem& sprites = level_state.sprites();
	if ( timer_ == target_time_ )
	{
		if ( ( int )( type_ ) > 0 )
		{
			sprites.spawn( std::make_unique<AnchorMissileSprite> ( xPixels(), yPixels(), ( Direction::Simple )( type_ ) ) );
		}
		else
		{
			sprites.spawn( std::make_unique<AnchorMissileSprite> ( xPixels(), yPixels(), Direction::Simple::LEFT ) );
			sprites.spawn( std::make_unique<AnchorMissileSprite> ( xPixels(), yPixels(), Direction::Simple::RIGHT ) );
		}
		timer_ = 0;
		target_time_ = newTargetTime();
	}
	else
	{
		++timer_;
	}
};

void SpawnAnchorMissileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};
