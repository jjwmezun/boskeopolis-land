#include "truck_platform_sprite.hpp"
#include "collision.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"

TruckPlatformSprite::TruckPlatformSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/truck-platform.png" ), x, y, 64, 6, { SpriteType::TRUCK_PLATFORM }, 500, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false )
{
	jump_lock_ = true;
};

TruckPlatformSprite::~TruckPlatformSprite() {};

void TruckPlatformSprite::customUpdate( LevelState& level_state )
{
	if ( hit_box_.y <= original_hit_box_.y - 176000 )
	{
		fullStopY();
	}
	else if ( !jump_lock_ )
	{
		moveUp();
	}
};

void TruckPlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};

void TruckPlatformSprite::reset()
{
	hit_box_ = original_hit_box_;
	jump_lock_ = true;
	fullStopY();
};

void TruckPlatformSprite::render( Camera& camera, bool priority )
{
	if ( !priority )
	{
		Render::renderObject
		(
			"sprites/truck-platform.png",
			{ 0, 6, 64, 1 },
			{ xPixels(), yPixels() + 6, 64, 10 + ( Unit::SubPixelsToPixels( original_hit_box_.y ) - yPixels() ) },
			SDL_FLIP_NONE,
			0.0,
			255,
			&camera
		);
	}
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
};
