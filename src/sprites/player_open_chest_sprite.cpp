#include "audio.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_state.hpp"
#include "player_graphics.hpp"
#include "player_open_chest_sprite.hpp"
#include "sprite_graphics.hpp"
#include "unit.hpp"

static constexpr int TIMER_LIMIT = 30;
static constexpr int CHEST_OPENING_SPEED = 2;
static constexpr int NUMBER_OF_TREASURE_CHEST_FRAMES = 8;
static constexpr int TREASURE_CHEST_GFX_WIDTH = 28;
static constexpr int CHEST_OPENING_ANIMATION_END = NUMBER_OF_TREASURE_CHEST_FRAMES * CHEST_OPENING_SPEED;

PlayerOpenChestSprite::PlayerOpenChestSprite( int x, int y, Direction::Horizontal dir )
:
	Sprite
	(
		std::make_unique<SpriteGraphics> ( Inventory::getPlayerCostume(), 0, 0, ( ( dir == Direction::Horizontal::RIGHT ) ? true : false ), false, 0, -1, -2, 2, 4 ),
		x,
		y,
		14,
		23,
		{},
		160,
		2000,
		1000,
		6000,
		dir,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::GROUNDED,
		CameraMovement::PERMANENT,
		false,
		true,
		true,
		false,
		.2
	),
	state_ ( POCState::INIT ),
	timer_ ( 0 )
{};

PlayerOpenChestSprite::~PlayerOpenChestSprite() {};

bool PlayerOpenChestSprite::animationChange() const
{
	return timer_ % Unit::DEFAULT_ANIMATION_SPEED == 0;
};

int PlayerOpenChestSprite::getAnimationFrame( int speed ) const
{
	return ( int )( floor( timer_ / speed ) );
};

void PlayerOpenChestSprite::walkAnimation()
{
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;

	if ( animationChange() )
	{
		const int FRAME = getAnimationFrame() % 4;
		switch ( FRAME )
		{
			case 0:
				graphics_->current_frame_x_ = 16;
			break;
			case 2:
				graphics_->current_frame_x_ = 32;
			break;
			default:
				graphics_->current_frame_x_ = 0;
			break;
		}
	}
	++timer_;
};

void PlayerOpenChestSprite::customUpdate( LevelState& level_state )
{
	switch ( state_ )
	{
		case ( POCState::INIT ):
			if ( timer_ >= TIMER_LIMIT )
			{
				timer_ = 0;
				state_ = POCState::START_MOVING;
			}
			else
			{
				++timer_;
			}
		break;
		case ( POCState::MOVE_LEFT ):
			moveLeft();
			walkAnimation();
		break;
		case ( POCState::MOVE_RIGHT ):
			moveRight();
			walkAnimation();
		break;
		case ( POCState::STOP ):
			fullStopX();
			state_ = POCState::OPENING;
			Audio::playSound( Audio::SoundType::CHEST_LOCK );
			graphics_->current_frame_x_ = 128;
			graphics_->current_frame_y_ = 26;
		break;
		case ( POCState::OPENING ):
			if ( timer_ < 60 )
			{
				if ( timer_ % 16 < 8 )
				{
					hit_box_.x -= 500;
				}
				else
				{
					hit_box_.x += 500;
				}
				++timer_;
			}
			else
			{
				graphics_->current_frame_y_ = 0;
				state_ = POCState::WALKING_AWAY;
			}
		break;
		case ( POCState::WALKING_AWAY ):
			moveRight();
			walkAnimation();
		break;
		case ( POCState::TURNING_TOWARD_CHEST ):
			fullStopX();
			graphics_->flip_x_ = false;
			graphics_->current_frame_x_ = 0;
			timer_ = -1;
			state_ = POCState::CHEST_OPENING;
		break;
		case ( POCState::CHEST_OPENING ):
			if ( timer_ == CHEST_OPENING_ANIMATION_END )
			{
				state_ = POCState::RISING_TREASURE;
				Audio::changeSong( "keycane", false );
				break;
			}
			else if ( timer_ > CHEST_OPENING_ANIMATION_END / 2 )
			{
				graphics_->current_frame_x_ = 5 * 16;
			}
			++timer_;
		break;
		case ( POCState::RISING_TREASURE ):
		break;
	}

	if ( Input::pressed( Input::Action::CONFIRM ) || Input::pressed( Input::Action::CANCEL ) || Input::pressed( Input::Action::MENU ) )
	{
		level_state.events().win();
	}
};

void PlayerOpenChestSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::TREASURE_CHEST ) )
	{
		switch ( state_ )
		{
			case ( POCState::START_MOVING ):
				if ( tooFarLeft( them ) )
				{
					state_ = POCState::MOVE_RIGHT;
				}
				else if ( tooFarRight( them ) )
				{
					state_ = POCState::MOVE_LEFT;
				}
				else
				{
					state_ = POCState::STOP;
				}
			break;
			case ( POCState::MOVE_LEFT ):
				if ( !tooFarRight( them ) )
				{
					state_ = POCState::STOP;
				}
			break;
			case ( POCState::MOVE_RIGHT ):
				if ( !tooFarLeft( them ) )
				{
					state_ = POCState::STOP;
				}
			break;
			case ( POCState::WALKING_AWAY ):
				if ( hit_box_.x >= them.rightSubPixels() )
				{
					state_ = POCState::TURNING_TOWARD_CHEST;
				}
			break;
			case ( POCState::CHEST_OPENING ):
				if ( them.jump_start_speed_ == 0 )
				{
					them.jump_start_speed_ = 1;
				}
			break;
			case ( POCState::RISING_TREASURE ):
			break;
		}
	}
	else if ( them.hasType( SpriteType::CLOUD_MONSTER ) )
	{
		them.kill();
	}
};

bool PlayerOpenChestSprite::tooFarLeft( const Sprite& them )
{
	return hit_box_.x < them.centerXSubPixels() - 6000;
};

bool PlayerOpenChestSprite::tooFarRight( const Sprite& them )
{
	return hit_box_.x > them.centerXSubPixels() - 3000;
};
