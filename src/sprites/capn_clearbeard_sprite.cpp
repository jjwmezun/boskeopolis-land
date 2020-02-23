#include "capn_clearbeard_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

CapnClearbeardSprite::CapnClearbeardSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/capn-clearbeard.png" ), x, y, 18, 30, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1000, 8000, 2000, 5000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	jump_timer_ ( 0 ),
	move_ ( true )
{
	jump_lock_ = false;
};

CapnClearbeardSprite::~CapnClearbeardSprite() {};

void CapnClearbeardSprite::customUpdate( LevelState& level_state )
{
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;

	if ( move_ )
	{
		moveInDirectionX();
	}

	if ( collide_left_ || collide_left_prev_ )
	{
		//direction_x_ = Direction::Horizontal::RIGHT;
		move_ = false;
		acceleration_x_ = 0;
	}
	else if ( collide_right_ || collide_right_prev_ )
	{
		//direction_x_ = Direction::Horizontal::LEFT;
		move_ = false;
		acceleration_x_ = 0;
	}
};

void CapnClearbeardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
