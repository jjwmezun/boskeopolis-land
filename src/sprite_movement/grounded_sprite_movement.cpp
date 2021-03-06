#include "collision.hpp"
#include "sprite.hpp"
#include "grounded_sprite_movement.hpp"

GroundedSpriteMovement::GroundedSpriteMovement()
:
	SpriteMovement( Type::GROUNDED )
{};

GroundedSpriteMovement::GroundedSpriteMovement( Type type )
:
	SpriteMovement( type )
{};

void GroundedSpriteMovement::moveUp( Sprite& sprite ) const
{
};

void GroundedSpriteMovement::moveDown( Sprite& sprite ) const
{
};

void GroundedSpriteMovement::jump( Sprite& sprite ) const
{
	sprite.top_speed_upward_ = sprite.jump_top_speed_;

	if ( !sprite.onGround() || !sprite.jump_lock_ )
	{
		if ( sprite.canJump() )
		{
			startJump( sprite );
		}

		if ( sprite.jump_start_ && !sprite.jump_end_ )
		{
			sprite.is_jumping_ = true;
		}

		if ( sprite.isJumping() )
		{
			sprite.acceleration_y_ = -sprite.jump_start_speed_;
		}
	}

	if ( sprite.isUpsideDown() )
	{
		if ( sprite.vy_ >= -sprite.jump_top_speed_ )
		{
			sprite.jump_start_ = false;
		}
	}
	else
	{
		if ( sprite.vy_ <= -sprite.jump_top_speed_ )
		{
			sprite.jump_start_ = false;
		}
	}
};

void GroundedSpriteMovement::bounce( Sprite& sprite, int amount ) const
{
	if ( sprite.bounce_height_ == 0 )
		sprite.bounce_height_ = Unit::PixelsToSubPixels( amount );

	sprite.is_bouncing_ = true;
	sprite.acceleration_y_ = -sprite.jump_start_speed_ * 8;

	sprite.top_speed_upward_ = sprite.bounce_height_;

	if
	(
		( sprite.isUpsideDown() && sprite.vy_ >= -sprite.bounce_height_) ||
		( !sprite.isUpsideDown() && sprite.vy_ <= -sprite.bounce_height_ )
	)
	{
		sprite.is_bouncing_ = false;
		sprite.bounce_height_ = 0;
	}
};

void GroundedSpriteMovement::position( Sprite& sprite ) const
{
	if ( !sprite.onGround() )
	{
		const int top_speed_modifier = ( sprite.isRunning() ) ? sprite.top_speed_run_ : sprite.top_speed_walk_;
		sprite.top_speed_ = top_speed_modifier * 0.75;
	}

	if ( sprite.isDucking() && !sprite.slide_jump_ )
	{
		sprite.acceleration_x_ /= 1.5;
	}

	if ( !sprite.onGround() && !sprite.isJumping() && !sprite.on_ladder_ && !sprite.is_bouncing_ )
	{
		sprite.bounce_height_ = 0;
		sprite.acceleration_y_ = sprite.fall_start_speed_;
	}

	sprite.top_speed_downward_ = sprite.fall_top_speed_;

	if ( sprite.onGround() )
	{
		sprite.slide_jump_ = false;
	}

	if ( sprite.on_ground_padding_.on() )
	{
		sprite.on_ground_padding_.update();
	}

	if ( sprite.on_ground_prev_ && !sprite.on_ground_ && !sprite.isJumping() )
	{
		sprite.on_ground_padding_.stop();
		sprite.on_ground_padding_.start();
	}
	else if ( sprite.on_ground_ || sprite.isJumping() )
	{
		sprite.on_ground_padding_.stop();
	}


	// UNIVERSAL
	if ( !sprite.isJumping() )
	{
		sprite.jump_end_ = true;
	}

	sprite.is_jumping_ = false;
	sprite.on_ground_prev_ = sprite.on_ground_;
	sprite.on_ground_ = false;
	sprite.is_bouncing_prev_ = sprite.is_bouncing_;

	// If not moving anymo', start slowing down.
	if ( sprite.acceleration_x_ == 0 )
	{
		sprite.vx_ /= sprite.traction_;
	}
};

void GroundedSpriteMovement::startJump( Sprite& sprite ) const
{
	sprite.jump_start_ = true;
	sprite.jump_end_ = false;
	sprite.vy_ = 0;
	sprite.on_ground_ = false;
};

void GroundedSpriteMovement::collideStopYBottom( Sprite& sprite, int overlap ) const
{
	if ( sprite.isUpsideDown() )
	{
		sprite.acceleration_y_ = 0;
		sprite.hit_box_.y += sprite.vy_;
		sprite.vy_ = 0;
		sprite.hit_box_.y -= overlap;
		sprite.jump_end_ = true;
	}
	else
	{
		if ( sprite.vy_ > 0 )
		{
			sprite.acceleration_y_ = 0;

			if ( !sprite.onGround() )
			{
				sprite.hit_box_.y -= overlap;
			}

			sprite.on_ground_ = true;
		}

		if ( sprite.on_ladder_ )
		{
			sprite.hit_box_.y -= sprite.LADDER_SPEED;
		}
	}

	sprite.collide_top_ = true;
};

void GroundedSpriteMovement::collideStopYTop( Sprite& sprite, int overlap ) const
{
	if ( sprite.isUpsideDown() )
	{
		if ( sprite.vy_ < 0 )
		{
			sprite.acceleration_y_ = 0;

			if ( !sprite.onGround() )
			{
				sprite.hit_box_.y += overlap;
			}

			sprite.on_ground_ = true;
		}

		if ( sprite.on_ladder_ )
		{
			sprite.hit_box_.y += sprite.LADDER_SPEED;
		}
	}
	else
	{
		sprite.acceleration_y_ = 0;
		sprite.hit_box_.y += sprite.vy_;
		sprite.vy_ = 0;
		sprite.hit_box_.y += overlap;
		sprite.jump_end_ = true;
	}
	sprite.collide_bottom_ = true;
};

void GroundedSpriteMovement::collideStopAny( Sprite& sprite, const Collision& collision ) const
{
	if ( collision.collideLeft() )
	{
		collideStopXLeft( sprite, collision.overlapXLeft() );
	}

	if ( collision.collideRight() )
	{
		collideStopXRight( sprite, collision.overlapXRight() );
	}

	if ( collision.collideBottom() )
	{
		collideStopYBottom( sprite, collision.overlapYBottom() );
	}

	if ( collision.collideTop() )
	{
		collideStopYTop( sprite, collision.overlapYTop() );
	}
};

const Collision GroundedSpriteMovement::testBlockCollision( const Sprite& me, const sdl2::SDLRect& them ) const
{
	int overlap_x_left   = 0;
	int overlap_x_right  = 0;
	int overlap_y_top    = 0;
	int overlap_y_bottom = 0;

	static constexpr int LEFT_RIGHT_PADDING = 4000;
	// Keep character from catching on walls moving vertically.
	if
	(
		me.leftSubPixels() + LEFT_RIGHT_PADDING < them.right() &&
		me.rightSubPixels() - LEFT_RIGHT_PADDING > them.left() &&
		me.topSubPixels() < them.bottom() &&
		me.bottomSubPixels() > them.top()
	)
	{
		if ( me.centerYSubPixels() > them.centerHeight() )
			overlap_y_top = them.bottom() - me.topSubPixels();
		else
			overlap_y_bottom = me.bottomSubPixels() - them.top();
	}

	// But allow character to stand on the tip-ends o' blocks.

	if ( !me.isUpsideDown() && me.centerYSubPixels() < them.centerHeight() )
	{
		if
		(
			me.leftSubPixels() + 1000 < them.right() &&
			me.rightSubPixels() - 3000 > them.left() &&
			me.topSubPixels() < them.bottom() &&
			me.bottomSubPixels() > them.top()
		)
		{
			overlap_y_bottom = me.bottomSubPixels() - them.top();
		}
	}
	else if ( me.isUpsideDown() && me.centerYSubPixels() > them.centerHeight() )
	{
		if
		(
			me.leftSubPixels() + 1000 < them.right() &&
			me.rightSubPixels() - 3000 > them.left() &&
			me.topSubPixels() < them.bottom() &&
			me.bottomSubPixels() > them.top()
		)
		{
			overlap_y_top = them.bottom() - me.topSubPixels();
		}
	}

	// 8000 padding needed to keep Autumn from thunking sideways into ceiling while hitting it from below.
	int top_padding = 0;
	if ( !me.on_ground_prev_ )
	{
		top_padding = 8000;
	}

	if
	(
		me.leftSubPixels() < them.right() &&
		me.rightSubPixels() > them.left() &&
		(
			( me.isUpsideDown() )
				? me.topSubPixels() + 4000 < them.bottom()
				: me.topSubPixels() + top_padding < them.bottom()
		) &&
		(
			( me.isUpsideDown() )
				? me.bottomSubPixels() - top_padding > them.top()
				: me.bottomSubPixels() - 4000 > them.top()
		)
		// Keep character from getting caught on sides o' floor blocks.
	)
	{
		if ( me.centerXSubPixels() < them.centerWidth() )
			overlap_x_right = me.rightSubPixels() - them.left();
		else if ( me.centerXSubPixels() > them.centerWidth() )
			overlap_x_left = them.right() - me.leftSubPixels();
	}

	return Collision( overlap_x_left, overlap_x_right, overlap_y_top, overlap_y_bottom );
};
