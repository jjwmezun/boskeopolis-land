#include "choque_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"

ChoqueSprite::ChoqueSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/choque.png" ), x, y, 24, 24, {}, 100, 1500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
still_ ( true )
{
	fall_start_speed_ = gravity_start_speed_ * 1.5;
	fall_top_speed_ = gravity_top_speed_ * 1.5;
};

ChoqueSprite::~ChoqueSprite() {};

void ChoqueSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( collide_top_ )
	{
		changeMovement( SpriteMovement::Type::FLOATING );
	}
	
	if ( hasMovementType( SpriteMovement::Type::FLOATING ) && !still_ )
	{
		moveUp();
	}

	if ( collide_bottom_ )
	{
		still_ = true;
	}
};

void ChoqueSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if 
	(
		them.hasType( SpriteType::HERO ) &&
		still_                           &&
		them.hit_box_.x > hit_box_.x - Unit::BlocksToSubPixels( 2 ) &&
		them.rightSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 2 )
	)
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
		still_ = false;
	}
	
	if ( their_collision.collideAny() )
	{
		if ( !their_collision.collideTop() )
		{
			them.collideStopAny( their_collision );
		}
		
		if ( their_collision.collideBottom() )
		{
			them.hit_box_.y += vy_;
		}
		
		if
		(
			them.hasType( SpriteType::HERO ) &&
			( ( their_collision.collideBottom() && them.collide_bottom_ ) ||
			( their_collision.collideTop() && them.collide_top_ ) )
		)
		{
			health.hurt();
		}
	}
};