#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "mezun_math.hpp"
#include "stronger_cowpoker_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

StrongerCowpokerSprite::StrongerCowpokerSprite( int x, int y, int map_id )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/cowpoker.png", 0, 74, false, false, 0, true, -1, -3, 2, 4 ), x, y, 15, 22, { SpriteType::DEATH_COUNT }, 500, 1000, jumpHeight(), jumpHeight(), Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true, false, .2, false, false, map_id ),
	awake_ ( false ),
	jump_ ( false ),
	is_shooting_ ( false ),
	hp_ ( 3 ),
	invincibility_ ( 0 ),
	move_type_ ( moveType() ),
	move_time_ ( moveTime() ),
	move_counter_ ( 0 ),
	jump_time_ ( jumpTime() ),
	jump_counter_ ( 0 ),
	throw_time_ ( 0 ),
	throw_counter_ ( 0 ),
	is_shooting_counter_ ( 0 )
{
	jump_lock_ = false;
};

StrongerCowpokerSprite::~StrongerCowpokerSprite() {};

void StrongerCowpokerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( awake_ )
	{
		if ( invincibility_ % 4 == 1 )
		{
			graphics_->visible_ = false;
		}
		else
		{
			graphics_->visible_ = true;

			switch ( direction_x_ )
			{
				case ( Direction::Horizontal::RIGHT ):
					graphics_->flip_x_ = true;
				break;

				default:
					graphics_->flip_x_ = false;
				break;
			}
		}

		if ( is_shooting_ )
		{
			graphics_->current_frame_x_ = 17;
		}
		else
		{
			graphics_->current_frame_x_ = 0;
		}

		if ( is_shooting_ )
		{
			if ( is_shooting_counter_ >= 8 )
			{
				is_shooting_ = false;
				is_shooting_counter_ = 0;
			}
			else
			{
				++is_shooting_counter_;
			}
		}

		handleMovement();
		handleJumping();
		handleThrowing( sprites );

		if ( invincibility_ > 0 )
		{
			--invincibility_;
		}
	}
	else
	{
		if ( events.trainDoorPartlyOpen() )
		{
			awake_ = true;
		}
	}
};

void StrongerCowpokerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( awake_ )
	{
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( them.xSubPixels() > rightSubPixels() )
			{
				direction_x_ = Direction::Horizontal::RIGHT;
			}
			else
			{
				direction_x_ = Direction::Horizontal::LEFT;
			}

			if ( them.collideBottomOnly( their_collision, *this ) )
			{
				hurt();
				them.bounce();
			}
			else if ( their_collision.collideAny() && !isDead() )
			{
				health.hurt();
			}
		}
		else if ( them.hasType( SpriteType::HEROS_BULLET ) )
		{
			if ( their_collision.collideAny() )
			{
				hurt();
			}
		}
	}
};

void StrongerCowpokerSprite::hurt()
{
	if ( invincibility_ == 0 )
	{
		--hp_;
		Audio::playSound( Audio::SoundType::BOP );
		invincibility_ = 60;

		if ( hp_ <= 0 )
		{
			kill();
		}
	}
};

int StrongerCowpokerSprite::jumpHeight() const
{
	return mezun::randInt( 8000, 3000 );
};

int StrongerCowpokerSprite::jumpTime() const
{
	return mezun::randInt( 180, 30 );
};

void StrongerCowpokerSprite::handleJumping()
{
	if ( jump_counter_ >= jump_time_ && onGround() )
	{
		jump();
		jump_counter_ = 0;
		jump_time_ = jumpTime();
		jump_start_speed_ = jumpHeight();
		jump_top_speed_ = jumpHeight();
	}
	++jump_counter_;
};

int StrongerCowpokerSprite::moveTime() const
{
	return mezun::randInt( 32, 8 );
};

int StrongerCowpokerSprite::moveType() const
{
	return mezun::randInt( 3, 0 );
}

void StrongerCowpokerSprite::handleMovement()
{
	if ( move_counter_ >= move_time_ )
	{
		move_type_ = moveType();
		move_time_ = moveTime();
		move_counter_ = 0;
	}

	switch ( move_type_ )
	{
		case ( 3 ):
			if ( !collide_left_ && !collide_left_prev_ )
			{
				hit_box_.x -= 1000;
			}
		break;

		case ( 2 ):
			if ( !collide_right_ && !collide_right_prev_ )
			{
				hit_box_.x += 1000;
			}
		break;

		default:
			fullStopX();
		break;
	}

	++move_counter_;
};

int StrongerCowpokerSprite::throwTime() const
{
	return mezun::randInt( 50, 5 );
};

void StrongerCowpokerSprite::handleThrowing( SpriteSystem& sprites )
{
	if ( throw_counter_ >= throw_time_ )
	{
		int bullet_y = ( mezun::randBool() ) ? yPixels() + 2 : yPixels() + 10;
		sprites.spawnEnemyBullet( centerXPixels(), bullet_y, Direction::horizontalToSimple( direction_x_ ) );
		throw_counter_ = 0;
		throw_time_ = throwTime();
		is_shooting_ = true;
	}
	++throw_counter_;
};
