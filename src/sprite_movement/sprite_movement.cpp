#include "collision.hpp"
#include "sprite.hpp"
#include "sprite_movement.hpp"

SpriteMovement::SpriteMovement( Type type )
:
	type_ ( type )
{};
SpriteMovement::~SpriteMovement() {};

SpriteMovement::Type SpriteMovement::type() const { return type_; };

bool SpriteMovement::hasType( Type type ) const
{
	return type == type_;
};

void SpriteMovement::moveLeft( Sprite& sprite ) const
{
	sprite.acceleration_x_ = -sprite.start_speed_;
	sprite.direction_x_ = Direction::Horizontal::LEFT;
	sprite.direction_ = Direction::Simple::LEFT;
};

void SpriteMovement::moveRight( Sprite& sprite ) const
{
	sprite.acceleration_x_ = sprite.start_speed_;
	sprite.direction_x_ = Direction::Horizontal::RIGHT;
	sprite.direction_ = Direction::Simple::RIGHT;
};

void SpriteMovement::moveUp( Sprite& sprite ) const
{
	sprite.acceleration_y_ = -sprite.start_speed_;
	sprite.direction_y_ = Direction::Vertical::UP;
	sprite.direction_ = Direction::Simple::UP;
};

void SpriteMovement::moveDown( Sprite& sprite ) const
{
	sprite.acceleration_y_ = sprite.start_speed_;
	sprite.direction_y_ = Direction::Vertical::DOWN;
	sprite.direction_ = Direction::Simple::DOWN;
};

void SpriteMovement::jump( Sprite& sprite ) const
{
};

void SpriteMovement::bounce( Sprite& sprite, int amount ) const
{
};

void SpriteMovement::position( Sprite& sprite ) const
{
	sprite.top_speed_upward_ = sprite.top_speed_downward_ = ( sprite.isUpsideDown() ) ? -sprite.top_speed_ : sprite.top_speed_;

	// If not moving anymo', start slowing down.
	if ( sprite.acceleration_x_ == 0 )
	{
		sprite.vx_ /= sprite.traction_;
	}
	if ( sprite.acceleration_y_ == 0 )
	{
		sprite.vy_ /= sprite.traction_;
	}
};

void SpriteMovement::collideStopXLeft( Sprite& sprite, int overlap ) const
{
	if ( sprite.vx_ < 0 )
	{
		//sprite.stopX();
		sprite.hit_box_.x -= sprite.vx_;
		sprite.vx_ = -( sprite.vx_ * sprite.bounce_ );
	}
	sprite.collide_left_ = true;
};

void SpriteMovement::collideStopXRight( Sprite& sprite, int overlap ) const
{
	if ( sprite.vx_ > 0 )
	{
		//sprite.stopX();
		sprite.hit_box_.x -= sprite.vx_;
		sprite.vx_ = -( sprite.vx_ * sprite.bounce_ );
	}
	sprite.collide_right_ = true;
};

void SpriteMovement::collideStopYBottom( Sprite& sprite, int overlap ) const
{
	if ( sprite.vy_ > 0 )
	{
		//sprite.stopY();
		sprite.hit_box_.y -= sprite.vy_;
		sprite.vy_ = -( sprite.vy_ * sprite.bounce_ );
	}
	sprite.collide_top_ = true;
};

void SpriteMovement::collideStopYTop( Sprite& sprite, int overlap ) const
{
	if ( sprite.vy_ < 0 )
	{
		//sprite.stopY();
		sprite.hit_box_.y -= sprite.vy_;
		sprite.vy_ = -( sprite.vy_ * sprite.bounce_ );
	}
	sprite.collide_bottom_ = true;
};

void SpriteMovement::collideStopAny( Sprite& sprite, const Collision& collision ) const
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

const Collision SpriteMovement::testCollision( const Sprite& me, const sdl2::SDLRect& them ) const
{
	int overlap_x_left   = 0;
	int overlap_x_right  = 0;
	int overlap_y_top    = 0;
	int overlap_y_bottom = 0;

	if
	(
		me.leftSubPixels() < them.right() &&
		me.rightSubPixels() > them.left() &&
		me.topSubPixels() < them.bottom() &&
		me.bottomSubPixels() > them.top()
	)
	{
		if ( me.centerYSubPixels() > them.centerHeight() )
		{
			overlap_y_top = them.bottom() - me.topSubPixels();
		}
		else
		{
			overlap_y_bottom = me.bottomSubPixels() - them.top();
		}
	}

	if
	(
		me.leftSubPixels() < them.right() &&
		me.rightSubPixels() > them.left() &&
		me.topSubPixels() < them.bottom() &&
		me.bottomSubPixels() > them.top()
	)
	{
		if ( me.centerXSubPixels() < them.centerWidth() )
		{
			overlap_x_right = me.rightSubPixels() - them.left();
		}
		else if ( me.centerXSubPixels() > them.centerWidth() )
		{
			overlap_x_left = them.right() - me.leftSubPixels();
		}
	}
	return Collision( overlap_x_left, overlap_x_right, overlap_y_top, overlap_y_bottom );
};

const Collision SpriteMovement::testBlockCollision( const Sprite& me, const sdl2::SDLRect& them ) const
{
	int overlap_x_left   = 0;
	int overlap_x_right  = 0;
	int overlap_y_top    = 0;
	int overlap_y_bottom = 0;

	if
	(
		// Allows sprite to still move vertically, e'en if colliding with a block horizontally.
		me.leftSubPixels() + SMOOTH_MOVEMENT_PADDING < them.right() &&
		me.rightSubPixels() - SMOOTH_MOVEMENT_PADDING  > them.left() &&
		me.topSubPixels() < them.bottom() &&
		me.bottomSubPixels() > them.top()
	)
	{
		if ( me.centerYSubPixels() > them.centerHeight() )
			overlap_y_top = them.bottom() - me.topSubPixels();
		else
			overlap_y_bottom = me.bottomSubPixels() - them.top();
	}

	if
	(
		me.leftSubPixels() < them.right() &&
		me.rightSubPixels() > them.left() &&
		// Allows sprite to still move horizontally, e'en if colliding with a block vertically.
		me.topSubPixels() + SMOOTH_MOVEMENT_PADDING < them.bottom() &&
		me.bottomSubPixels() - SMOOTH_MOVEMENT_PADDING > them.top()
	)
	{
		if ( me.centerXSubPixels() < them.centerWidth() )
			overlap_x_right = me.rightSubPixels() - them.left();
		else if ( me.centerXSubPixels() > them.centerWidth() )
			overlap_x_left = them.right() - me.leftSubPixels();
	}

	return Collision( overlap_x_left, overlap_x_right, overlap_y_top, overlap_y_bottom );
};

std::string SpriteMovement::typeName() const
{
	switch ( type_ )
	{
		case ( Type::GROUNDED   ): { return "GROUNDED";   } break;
		case ( Type::FLOATING   ): { return "FLOATING";   } break;
		case ( Type::FLUTTERING ): { return "FLUTTERING"; } break;
		case ( Type::SWIMMING   ): { return "SWIMMING";   } break;
		case ( Type::ANGLED     ): { return "ANGLED";     } break;
		case ( Type::STUCK      ): { return "STUCK";      } break;
		default                  : { return "INVALID";    } break;
	}
}
