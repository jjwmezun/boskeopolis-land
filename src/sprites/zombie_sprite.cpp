#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "zombie_sprite.hpp"

static constexpr int HEAD_ACCELERATION_X = 25;
static constexpr int HEAD_MAX_SPEED = 2000;

static constexpr int MAX_HEIGHT = 13;

ZombieSprite::ZombieSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/zombie.png", 0, 16, false, false, 0.0, -5, 0, 9, 2 ), x, y, 9, 1, { SpriteType::ZOMBIE }, 50, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	state_ ( ZState::RISING ),
	timer_ ( 0 ),
	head_acceleration_x_ ( 0 ),
	head_vx_ ( 0 ),
	head_ ( Unit::PixelsToSubPixels( x - 3 ), 0, Unit::PixelsToSubPixels( 16 ), Unit::PixelsToSubPixels( 13 ) ),
	head_gfx_ ( "sprites/zombie.png", 0, 0, false, false, 0.0, -2, -3, 2, 3 )
{
	sprite_interact_ = false;
};

ZombieSprite::~ZombieSprite() {};

void ZombieSprite::customUpdate( LevelState& level_state )
{
	if ( rightPixels() < level_state.camera().x() - Unit::WINDOW_WIDTH_PIXELS || xPixels() > level_state.camera().right() + Unit::WINDOW_WIDTH_PIXELS )
	{
		kill();
		return;
	}

	switch ( state_ )
	{
		case ( ZState::RISING ):
		{
			if ( head_.y < hit_box_.y - Unit::PixelsToSubPixels( 13 ) )
			{
				if ( graphics_->y_adjustment_ == -1 )
				{
					hit_box_.h += Unit::PixelsToSubPixels( 1 );
					hit_box_.y -= Unit::PixelsToSubPixels( 1 );
					if ( hit_box_.h == Unit::PixelsToSubPixels( 13 ) )
					{
						sprite_interact_ = true;
						state_ = ZState::MOVING;
					}
				}
				else
				{
					--graphics_->y_adjustment_;
				}
				head_.y = hit_box_.y - Unit::PixelsToSubPixels( 13 );
			}
			else
			{
				head_.y -= Unit::PixelsToSubPixels( 1 );
			}
		}
		break;
		case ( ZState::MOVING ):
		{
			moveInDirectionX();
			head_.x = hit_box_.x - Unit::PixelsToSubPixels( 3 );
			if ( timer_ == 8 )
			{
				timer_ = 0;
				graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 18 : 0;
			}
			else
			{
				++timer_;
			}
		}
		break;
		case ( ZState::BEFORE_THROW ):
		{
			if ( timer_ == 16 )
			{
				timer_ = 0;
				state_ = ZState::THROWING;
				head_acceleration_x_ = ( ( direction_x_ == Direction::Horizontal::RIGHT ) ? 1 : -1 ) * HEAD_ACCELERATION_X;
			}
			else
			{
				++timer_;
			}
		}
		break;
		case ( ZState::THROWING ):
		{
			head_vx_ = std::max( -HEAD_MAX_SPEED, std::min( HEAD_MAX_SPEED, head_vx_ + head_acceleration_x_ ) );
			head_.x += head_vx_;
			const bool head_reached_end = ( direction_x_ == Direction::Horizontal::RIGHT )
				? head_.x > hit_box_.x - Unit::BlocksToSubPixels( 3 )
				: head_.x < hit_box_.x - Unit::BlocksToSubPixels( 3 );
			if ( head_reached_end )
			{
				state_ = ZState::PULLING;
			}
		}
		break;
		case ( ZState::PULLING ):
		{
			head_vx_ = std::max( -HEAD_MAX_SPEED, std::min( HEAD_MAX_SPEED, head_vx_ - head_acceleration_x_ ) );
			head_.x += head_vx_;
			const bool head_is_back = ( direction_x_ == Direction::Horizontal::RIGHT )
				? head_.x <= hit_box_.x - Unit::PixelsToSubPixels( 3 )
				: head_.x >= hit_box_.x - Unit::PixelsToSubPixels( 3 );
			if ( head_is_back )
			{
				head_.x = hit_box_.x - Unit::PixelsToSubPixels( 3 );
				head_vx_ = head_acceleration_x_ = 0;
				state_ = ZState::MOVING;
			}
		}
		break;
	}
	flipGraphicsOnRight();
	head_gfx_.flip_x_ = graphics_->flip_x_;
};

void ZombieSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( state_ == ZState::MOVING )
		{
			direction_x_ = ( them.centerXSubPixels() > centerXSubPixels() ) ? Direction::Horizontal::RIGHT : Direction::Horizontal::LEFT;
			if ( on_ground_ )
			{
				const bool near_player = ( direction_x_ == Direction::Horizontal::RIGHT )
					? them.hit_box_.x < rightSubPixels() + Unit::BlocksToSubPixels( 6 )
					: them.rightSubPixels() > hit_box_.x - Unit::BlocksToSubPixels( 6 );
				if ( near_player )
				{
					state_ = ZState::BEFORE_THROW;
					timer_ = 0;
					graphics_->current_frame_x_ = 0;
					fullStopX();
				}
			}
		}

		if ( state_ != ZState::RISING )
		{
			const Collision head_collision = them.testCollision( head_ );
			if ( them.collideBottomOnly( head_collision, head_ ) || them.collideBottomOnly( their_collision, *this ) )
			{
				kill();
				them.bounce();
				if ( them.movementType() != SpriteMovement::Type::SWIMMING )
				{
					level_state.inventory().bop();
				}
				Audio::playSound( Audio::SoundType::BOP );
			}
			if ( their_collision.collideAny() && level_state.events().testIsSlidingPreviously() )
			{
				kill();
				Audio::playSound( Audio::SoundType::BOP );
			}
			else if ( !isDead() && ( their_collision.collideAny() || head_collision.collideAny() ) )
			{
				level_state.health().hurt();
			}
		}
	}
};

void ZombieSprite::render( const Camera& camera ) const
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
	head_gfx_.render( Unit::SubPixelsToPixels( head_ ), &camera );
	Render::renderRectDebug( Unit::SubPixelsToPixels( hit_box_ ), { 255, 255, 255, 255 } );
};

void ZombieSprite::deathAction( LevelState& level_state )
{
	defaultDeathAction( level_state );
	head_.y = hit_box_.y - Unit::PixelsToSubPixels( 13 );
	head_gfx_.rotation_ += ( direction_x_ == Direction::Horizontal::RIGHT ) ? 5 : -5;
};