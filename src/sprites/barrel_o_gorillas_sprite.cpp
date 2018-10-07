#include "barrel_o_gorillas_sprite.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "gorilla_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static int getRandomTimeLimit()
{
	return mezun::randInt( 140, 40 );
}

BarrelOGorillasSprite::BarrelOGorillasSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/barrel-o-gorillas.png", 0, 0, false, false, 0, true, 0, -24, 0, 24 ), x, y + 24, 32, 8, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	top_hit_box_ ( Unit::PixelsToSubPixels({ x, y, 32, 8 }) ),
	right_hit_box_ ( Unit::PixelsToSubPixels({ x + 24, y, 8, 32 }) ),
	spawn_timer_ ( 0 ),
	spawn_limit_ ( mezun::randInt( 140, 40 ) )
{};

BarrelOGorillasSprite::~BarrelOGorillasSprite() {};

void BarrelOGorillasSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( spawn_timer_ >= spawn_limit_ )
	{
		sprites.spawn( std::make_unique<GorillaSprite> ( xPixels() + 8, yPixels() - 16 ) );
		spawn_limit_ = mezun::randInt( 80, 20 );
		spawn_timer_ = 0;
	}
	else
	{
		++spawn_timer_;
	}
};

void BarrelOGorillasSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( !them.hasType( SpriteType::PHASE_THROUGH ) )
	{
		Collision top_collision = movement_->testCollision( them, top_hit_box_ );
		Collision right_collision = movement_->testCollision( them, right_hit_box_ );
		them.collideStopAny( their_collision );
		them.collideStopAny( top_collision );
		them.collideStopAny( right_collision );
	}
};
