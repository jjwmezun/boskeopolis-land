#include "collision.hpp"
#include "sprite.hpp"
#include "fluttering_sprite_movement.hpp"

FlutteringSpriteMovement::FlutteringSpriteMovement()
:
	GroundedSpriteMovement( Type::FLUTTERING )
{};

void FlutteringSpriteMovement::jump( Sprite& sprite ) const
{
	if ( !sprite.jump_lock_ )
	{
		sprite.acceleration_y_ = -sprite.jump_start_speed_;
		sprite.is_jumping_ = true;
	}
};

void FlutteringSpriteMovement::position( Sprite& sprite ) const
{

	if ( !sprite.onGround() && !sprite.isJumping() )
	{
		sprite.acceleration_y_ = sprite.fall_start_speed_ / 2;
	}

	sprite.top_speed_upward_ = sprite.jump_top_speed_ / 2;
	sprite.top_speed_downward_ = sprite.fall_top_speed_ / 2;

	sprite.on_ground_prev_ = sprite.on_ground_;
	sprite.on_ground_ = false;
	sprite.is_jumping_ = false;

	// If not moving anymo', start slowing down.
	if ( sprite.acceleration_x_ == 0 )
		sprite.vx_ /= sprite.traction_;
};