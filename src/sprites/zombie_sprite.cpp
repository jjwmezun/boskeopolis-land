#include "zombie_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int HEAD_ACCELERATION_X = 25;
static constexpr int HEAD_MAX_SPEED = 2000;

ZombieSprite::ZombieSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 1, { SpriteType::ZOMBIE, SpriteType::ENEMY, SpriteType::BOPPABLE }, 50, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	state_ ( ZState::RISING ),
	timer_ ( 0 ),
	head_acceleration_x_ ( 0 ),
	head_vx_ ( 0 ),
	head_ ( 0, 0, Unit::BlocksToSubPixels( 1 ), Unit::BlocksToSubPixels( 1 ) )
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
			hit_box_.h += Unit::PixelsToSubPixels( 1 );
			if ( hit_box_.h == Unit::PixelsToSubPixels( 24 ) )
			{
				sprite_interact_ = true;
				state_ = ZState::MOVING;
			}
		}
		break;
		case ( ZState::MOVING ):
		{
			moveInDirectionX();
		}
		break;
		case ( ZState::BEFORE_THROW ):
		{
			if ( timer_ == 16 )
			{
				timer_ = 0;
				head_.x = hit_box_.x;
				head_.y = hit_box_.y;
				hit_box_.h -= Unit::BlocksToSubPixels( 1 );
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
				? head_.x > hit_box_.x + Unit::BlocksToSubPixels( 6 )
				: head_.x < hit_box_.x - Unit::BlocksToSubPixels( 6 );
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
				? head_.x < hit_box_.x
				: head_.x > hit_box_.x;
			if ( head_is_back )
			{
				state_ = ZState::MOVING;
				hit_box_.h += Unit::BlocksToSubPixels( 1 );
			}
		}
		break;
	}
};

void ZombieSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		switch ( state_ )
		{
			case ( ZState::RISING ):
			{
			}
			break;
			case ( ZState::MOVING ):
			{
				direction_x_ = ( them.centerXSubPixels() > centerXSubPixels() ) ? Direction::Horizontal::RIGHT : Direction::Horizontal::LEFT;
				const bool near_player = ( direction_x_ == Direction::Horizontal::RIGHT )
					? them.hit_box_.x < rightSubPixels() + Unit::BlocksToSubPixels( 6 )
					: them.rightSubPixels() > hit_box_.x - Unit::BlocksToSubPixels( 6 );
				if ( near_player )
				{
					state_ = ZState::BEFORE_THROW;
					fullStopX();
				}
			}
			break;
			case ( ZState::BEFORE_THROW ):
			{
			}
			break;
		}
	}
};
