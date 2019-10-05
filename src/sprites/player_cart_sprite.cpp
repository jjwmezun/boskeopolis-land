#include "audio.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "render.hpp"
#include "health.hpp"
#include "input.hpp"
#include "map.hpp"
#include "player_cart_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int JUMP_LIMIT = Unit::BlocksToSubPixels( 32 );

PlayerCartSprite::PlayerCartSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/autumn_cart.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 44, 44, { SpriteType::HERO }, 160, 5000, 1000, 7000, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, false, true, true, false, .8 ),
	reached_height_ ( false ),
	duck_data_ ( { true, 8, 8, -2, 4 } ),
	unduck_data_ ( { 8, 0, -2, 4 } )
{
	run();
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
		if ( !isDucking() )
		{
			jump();
			jump_lock_ = true;
		}

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
		jump_lock_ = false;
	}

	if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		duck( duck_data_ );
	}
	else if ( Input::released( Input::Action::MOVE_DOWN ) )
	{
		unduck( unduck_data_ );
	}

	if ( fellInBottomlessPit( lvmap ) )
	{
		kill();
	}

	invincibilityFlicker( health );
	camera.adjustCart( *this, lvmap );
	updateGraphics();
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

void PlayerCartSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	defaultDeathAction( camera );
	events.playDeathSoundIfNotAlreadyPlaying();
};

void PlayerCartSprite::updateGraphics()
{
	if ( is_ducking_ )
	{
		graphics_->current_frame_x_ = 48;
		graphics_->current_frame_y_ = 8;
	}
	else
	{
		switch ( direction_x_ )
		{
			case ( Direction::Horizontal::LEFT ):
			{
				graphics_->current_frame_x_ = 96;
				graphics_->current_frame_y_ = 0;
			}
			break;

			case ( Direction::Horizontal::RIGHT ):
			{
				graphics_->current_frame_x_ = 0;
				graphics_->current_frame_y_ = 0;
			}
			break;
		}
	}
};
