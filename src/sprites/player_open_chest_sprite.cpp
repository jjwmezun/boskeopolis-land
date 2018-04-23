#include "player_graphics.hpp"
#include "player_open_chest_sprite.hpp"
#include "unit.hpp"

static constexpr int TIMER_LIMIT = 30;

PlayerOpenChestSprite::PlayerOpenChestSprite( int x, int y, Direction::Horizontal dir )
:
	Sprite
	(
		std::make_unique<PlayerGraphics> ( "sprites/autumn.png" ),
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

void PlayerOpenChestSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( state_ )
	{
		case ( POCState::INIT ):
			if ( timer_ >= TIMER_LIMIT )
			{
				state_ = POCState::START_MOVING;
			}
			else
			{
				++timer_;
			}
		break;
		case ( POCState::MOVE_LEFT ):
			moveLeft();
		break;
		case ( POCState::MOVE_RIGHT ):
			moveRight();
		break;
		case ( POCState::STOP ):
			fullStopX();
			graphics_.reset( new SpriteGraphics( "sprites/autumn.png", 0, 0, false, false, 0, false, -1, -2, 2, 4 ) );
			state_ = POCState::OPENING;
		break;
		case ( POCState::OPENING ):
			graphics_->current_frame_x_ = 128;
			graphics_->current_frame_y_ = 26;
		break;
	}
};

void PlayerOpenChestSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
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
		}
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