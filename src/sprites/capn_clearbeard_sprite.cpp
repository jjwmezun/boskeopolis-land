#include "capn_clearbeard_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

CapnClearbeardSprite::CapnClearbeardSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/capn-clearbeard.png" ), x, y, 18, 30, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 50, 5000, 2000, 5000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	jump_timer_ ( 0 ),
	move_ ( true )
{
	jump_lock_ = false;
};

CapnClearbeardSprite::~CapnClearbeardSprite() {};

void CapnClearbeardSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;

	if ( move_ )
	{
		moveInDirectionX();
	}
	else
	{
		if ( jump_timer_ <= 0 && on_ground_ )
		{
			direction_x_ = Direction::switchHorizontal( direction_x_ );
			move_ = true;
		}
	}

	if ( collide_left_ || collide_left_prev_ || collide_right_ || collide_right_prev_ )
	{
		jump_timer_ = 15;
		move_ = false;
	}

	if ( jump_timer_ > 0 )
	{
		jump();
		--jump_timer_;
	}

	std::cout<<jump_timer_<<std::endl;
};

void CapnClearbeardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};
