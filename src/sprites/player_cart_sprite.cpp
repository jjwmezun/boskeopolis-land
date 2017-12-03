#include "audio.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "render.hpp"
#include "health.hpp"
#include "input.hpp"
#include "map.hpp"
#include "player_cart_sprite.hpp"
#include "player_cart_graphics.hpp"

PlayerCartSprite::PlayerCartSprite( int x, int y )
:
	Sprite( std::make_unique<PlayerCartGraphics> (), x, y, 44, 44, { SpriteType::HERO }, 160, 5000, 1000, 7000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, false, true, true, false, .8 )
{
	run();
	direction_x_ = Direction::Horizontal::RIGHT;
};

PlayerCartSprite::~PlayerCartSprite() {};

void PlayerCartSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( direction_x_ == Direction::Horizontal::RIGHT )
	{
		if ( collide_right_ )
		{
			health.hurt();
		}

		if ( collide_bottom_ )
		{
			health.hurt();
		}

		if ( acceleration_x_ < 0 )
		{
			acceleration_x_ = 0;
		}

		if ( vx_ < 0 )
		{
			vx_ = 0;
		}

		moveRight();
	}
	else if ( direction_x_ == Direction::Horizontal::LEFT )
	{
		if ( collide_left_ )
		{
			health.hurt();
		}

		if ( collide_bottom_ )
		{
			health.hurt();
		}

		if ( acceleration_x_ > 0 )
		{
			acceleration_x_ = 0;
		}

		if ( vx_ > 0 )
		{
			vx_ = 0;
		}

		moveLeft();
	}
	else
	{
		if ( !reached_height_ )
		{
			acceleration_x_ = 0;
			vx_ = 0;
			changeMovement( SpriteMovement::Type::FLOATING );
			acceleration_y_ = -jump_start_speed_;

			if ( topSubPixels() <= JUMP_LIMIT )
			{
				reached_height_ = true;
			}
		}
		else
		{
			changeMovement( SpriteMovement::Type::GROUNDED );

			if ( on_ground_prev_ )
			{
				direction_x_ = Direction::Horizontal::RIGHT;
			}
		}
	}

	if ( Input::held( Input::Action::JUMP ) )
	{
		jump();
		jump_lock_ = true;

		if ( is_bouncing_ )
		{
			bounce();
		}
		if ( on_ground_prev_ )
		{
			Audio::playSound( Audio::SoundType::JUMP );
		}
	}
	else
	{
		if ( is_bouncing_prev_ )
		{
			is_bouncing_ = false;
		}

		if ( !isDucking() )
		{
			jump_lock_ = false;
		}
	}

	if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		duck();
	}

	if ( Input::released( Input::Action::MOVE_DOWN ) )
	{
		unduck();
	}

	if ( isDucking() )
	{
		jump_lock_ = true;
	}

	if (  fellInBottomlessPit( lvmap ) )
	{
		kill();
	}

	invincibilityFlicker( health );
	camera.adjustCart( *this, lvmap );
};

void PlayerCartSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::ENEMY ) && my_collision.collideAny() )
	{
		if ( them.hasType( SpriteType::BOPPABLE ) )
		{
			if ( them.on_slope_ == Direction::Horizontal::LEFT && bottomSubPixels() < them.bottomSubPixels() )
			{
				them.kill();
				bounce();
			}
			else if ( bottomSubPixels() < them.centerYSubPixels() )
			{
				them.kill();
				bounce();
			}
			else if ( !them.isDead() )
			{
				health.hurt();
			}
		}
		else if ( !them.isDead() )
		{
			health.hurt();
		}
	}
};

void PlayerCartSprite::duck()
{
	// Hacky way to make player warp to the right position after height changes.
	if ( !isDucking() )
	{
		hit_box_.y += Unit::PixelsToSubPixels( 8 );
		//graphics_->y_adjustment_ = -5;
		//graphics_->h_adjustment_ = -16;
	}

	is_ducking_ = true;

	hit_box_.h = original_hit_box_.h - Unit::PixelsToSubPixels( 8 );
};

void PlayerCartSprite::unduck()
{
	// Hacky way to keep player from falling through ground after gaining height from unducking.
	if ( isDucking() )
	{
		hit_box_.y -= Unit::PixelsToSubPixels( 8 );
		//graphics_->y_adjustment_ = -1;
		//graphics_->h_adjustment_ = 2;
	}

	is_ducking_ = false;
	jump_lock_ = false;
	hit_box_.h = original_hit_box_.h;
};


void PlayerCartSprite::deathAction( Camera& camera, EventSystem& events )
{
	defaultDeathAction( camera );
	events.playDeathSoundIfNotAlreadyPlaying();
};