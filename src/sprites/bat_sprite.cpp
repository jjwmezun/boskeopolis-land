#include "bat_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

BatSprite::BatSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bat.png" ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 400, 1600, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	awake_ ( false )
{
	block_interact_ = false;
	graphics_->priority_ = true;
};

BatSprite::~BatSprite() {};

void BatSprite::customUpdate( LevelState& level_state )
{
	if ( awake_ )
	{
		switch ( direction_x_ )
		{
			case ( Direction::Horizontal::LEFT ):
				moveLeft();
			break;
			case ( Direction::Horizontal::RIGHT ):
				moveRight();
			break;
		}

		switch ( direction_y_ )
		{
			case ( Direction::Vertical::UP ):
				moveUp();
			break;
			case ( Direction::Vertical::DOWN ):
				moveDown();
			break;
		}
	}
};

void BatSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if
	(
		them.hit_box_.x > hit_box_.x - Unit::BlocksToSubPixels( 4 ) &&
		them.rightSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 4 ) &&
		them.hit_box_.y > hit_box_.y - Unit::BlocksToSubPixels( 4 ) &&
		them.bottomSubPixels() < bottomSubPixels() + Unit::BlocksToSubPixels( 4 )
	)
	{
		awake_ = true;
	}
	
	if ( awake_ )
	{
		if ( them.rightSubPixels() < hit_box_.x )
		{
			direction_x_ = Direction::Horizontal::LEFT;
		}
		else if ( them.hit_box_.x > rightSubPixels() )
		{
			direction_x_ = Direction::Horizontal::RIGHT;
		}

		if ( them.bottomSubPixels() < hit_box_.y )
		{
			direction_y_ = Direction::Vertical::UP;
		}
		else if ( them.hit_box_.y > bottomSubPixels() )
		{
			direction_y_ = Direction::Vertical::DOWN;
		}
	}
};