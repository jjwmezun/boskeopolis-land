#include "camera.hpp"
#include "crane_crate_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

CraneCrateSprite::CraneCrateSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/hook.png" ), x - 50, y - 160, 16, 16, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	circle_ ( original_hit_box_, original_hit_box_.x, original_hit_box_.y, 200000, 0.01, 1.25 ),
	prev_x_ ( original_hit_box_.x ),
	prev_y_ ( original_hit_box_.y )
{};

CraneCrateSprite::~CraneCrateSprite() {};

void CraneCrateSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	prev_x_ = hit_box_.x;
	prev_y_ = hit_box_.y;
	circle_.updatePosition();
	if ( direction_x_ == Direction::Horizontal::LEFT )
	{
		circle_.updateAngleCounterClockwise();
		if ( circle_.getAngle() <= 1.25 )
		{
			direction_x_ = Direction::Horizontal::RIGHT;
		}
	}
	else
	{
		circle_.updateAngleClockwise();
		if ( circle_.getAngle() >= 2.14 - 0.25 )
		{
			direction_x_ = Direction::Horizontal::LEFT;
		}
	}

	hit_box_ = circle_.getPosition();
};

void CraneCrateSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		/*
		if ( their_collision.collideBottom() )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			them.hit_box_.y += ( hit_box_.y - prev_y_ );
			them.hit_box_.x += ( hit_box_.x - prev_x_ );
		}
		else
		{
			them.collideStopAny( their_collision );
		}*/
	}
};


void CraneCrateSprite::render( Camera& camera, bool priority )
{
	if ( !priority )
	{
		sdl2::SDLRect dest = camera.relativeRect( { xPixels(), yPixels() - 192, 16, 192 } );
		sdl2::SDLRect src = { 0, 16, 16, 192 };
		Render::renderObject( "sprites/hook.png", src, dest );
		graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	}
};
