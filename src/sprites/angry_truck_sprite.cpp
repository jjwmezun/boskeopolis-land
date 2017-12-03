#include "angry_truck_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

AngryTruckSprite::AngryTruckSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/angry_truck.png" ), x, y, 48, 32, {}, 2000, 2000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

AngryTruckSprite::~AngryTruckSprite() {};

void AngryTruckSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	/*
	if ( direction_x_ == Direction::Horizontal::RIGHT )
	{
		graphics_->flip_x_ = true;
		moveRight();
	}
	else
	{
		graphics_->flip_x_ = false;
		moveLeft();
	}

	if ( rightSubPixels() > original_hit_box_.right() + Unit::BlocksToSubPixels( 4 ) )
	{
		direction_x_ = Direction::Horizontal::LEFT;
	}
	if ( hit_box_.x < original_hit_box_.x - Unit::BlocksToSubPixels( 4 ) )
	{
		direction_x_ = Direction::Horizontal::RIGHT;
	}
	*/	
	graphics_->flip_x_ = true;
	moveRight();
};

void AngryTruckSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		them.hit_box_.x += vx_;
		
		if ( them.hasType( SpriteType::HERO ) && !their_collision.collideBottom() )
		{
			health.hurt();
		}
	}
};