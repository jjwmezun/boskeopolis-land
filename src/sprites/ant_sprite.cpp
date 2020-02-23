#include "ant_sprite.hpp"
#include "block_system.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int MOVE_SPEED = 1000;
static constexpr Direction::Rotation START_DIRECTION = Direction::Rotation::RIGHT;
static constexpr int SLOPE_LENGTH = 20;
static constexpr int ANIMATION_SPEED = 8;

AntSprite::AntSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/ant.png", 0, 0, false, false, 0, false, -1, -3, 2, 5 ), x, y, 22, 9, { SpriteType::BOPPABLE, SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, false ),
	direction_ ( START_DIRECTION ),
	slope_timer_ ( 0 )
{};

AntSprite::~AntSprite() {};

void AntSprite::customUpdate( LevelState& level_state )
{
	BlockSystem& blocks = level_state.blocks();
	switch ( direction_ )
	{
		case ( Direction::Rotation::UP ):
		{
			hit_box_.y -= MOVE_SPEED;
			if ( !blocks.blocksInTheWay( { rightSubPixels(), centerYSubPixels() - 4000, 4000, 4000 }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Rotation::UP_RIGHT;
			}
		}
		break;
		case ( Direction::Rotation::UP_RIGHT ):
		{
			hit_box_.y -= MOVE_SPEED;
			hit_box_.x += MOVE_SPEED;
			testSlopeChange( Direction::Rotation::RIGHT );
		}
		break;
		case ( Direction::Rotation::RIGHT ):
		{
			hit_box_.x += MOVE_SPEED;
			if ( !blocks.blocksInTheWay( { centerXSubPixels() + 4000, bottomSubPixels(), 4000, 4000 }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Rotation::DOWN_RIGHT;
			}
		}
		break;
		case ( Direction::Rotation::DOWN_RIGHT ):
		{
			hit_box_.y += MOVE_SPEED;
			hit_box_.x += MOVE_SPEED;
			testSlopeChange( Direction::Rotation::DOWN );
		}
		break;
		case ( Direction::Rotation::DOWN ):
		{
			hit_box_.y += MOVE_SPEED;
			if ( !blocks.blocksInTheWay( { hit_box_.x - 4000, centerYSubPixels() + 4000, 4000, 4000 }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Rotation::DOWN_LEFT;
			}
		}
		break;
		case ( Direction::Rotation::DOWN_LEFT ):
		{
			hit_box_.y += MOVE_SPEED;
			hit_box_.x -= MOVE_SPEED;
			testSlopeChange( Direction::Rotation::LEFT );
		}
		break;
		case ( Direction::Rotation::LEFT ):
		{
			hit_box_.x -= MOVE_SPEED;
			if ( !blocks.blocksInTheWay( { centerXSubPixels() - 4000, hit_box_.y - 4000, 4000, 4000 }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Rotation::UP_LEFT;
			}
		}
		break;
		case ( Direction::Rotation::UP_LEFT ):
		{
			hit_box_.y -= MOVE_SPEED;
			hit_box_.x -= MOVE_SPEED;
			testSlopeChange( Direction::Rotation::UP );
		}
		break;
	}

	if ( animation_timer_ >= ANIMATION_SPEED - 1 )
	{
		graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 24 : 0;
		animation_timer_ = 0;
	}
	else
	{
		++animation_timer_;
	}
};

void AntSprite::testSlopeChange( Direction::Rotation next_direction )
{
	if ( slope_timer_ >= SLOPE_LENGTH )
	{
		direction_ = next_direction;
		slope_timer_ = 0;
		switch ( next_direction )
		{
			case ( Direction::Rotation::UP    ): { graphics_->rotation_ = 270.0; } break;
			case ( Direction::Rotation::RIGHT ): { graphics_->rotation_ =   0.0; } break;
			case ( Direction::Rotation::DOWN  ): { graphics_->rotation_ =  90.0; } break;
			case ( Direction::Rotation::LEFT  ): { graphics_->rotation_ = 180.0; } break;
		}
	}
	else
	{
		++slope_timer_;
	}

	graphics_->rotation_ += floor( 90.0 / 16.0 );
};

void AntSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
