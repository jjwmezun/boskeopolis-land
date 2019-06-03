#include "crane_crate_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

CraneCrateSprite::CraneCrateSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 160, 48 ), x - 50, y - 160, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	circle_ ( original_hit_box_, original_hit_box_.x, original_hit_box_.y, 175000, 0.005, 1.25 ),
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
	std::cout<<circle_.getAngle()<<std::endl;;
	std::cout<<( int )( direction_x_ )<<std::endl;;
};

void CraneCrateSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		if ( their_collision.collideBottom() )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			them.hit_box_.y += ( hit_box_.y - prev_y_ );
			them.hit_box_.x += ( hit_box_.x - prev_x_ );
		}
		else
		{
			them.collideStopAny( their_collision );
		}
	}
};
