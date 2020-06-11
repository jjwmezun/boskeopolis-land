#include "collision.hpp"
#include "level_state.hpp"
#include "vertical_pike_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int MAX_GROW_SPEED = 8000;
static constexpr int GROW_ACCELERATION = 200;
static constexpr int WAIT_TIME = 32;

VerticalPikeSprite::VerticalPikeSprite( int x, int y, Direction::Vertical direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pike.png", 0, 0, false, false, 0, false, -1, -1, 2, 2 ), x, y, 10, 0, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, direction, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	grow_speed_ ( 0 ),
	state_ ( PikeState::WAIT ),
	wait_timer_ ( 0 )
{};

VerticalPikeSprite::~VerticalPikeSprite() {};

void VerticalPikeSprite::customUpdate( LevelState& level_state )
{
	switch ( state_ )
	{
		case ( PikeState::WAIT ):
		{
            ++wait_timer_;
			if ( wait_timer_ == WAIT_TIME )
			{
				wait_timer_ = 0;
				state_ = PikeState::GROW;
			}
		}
		break;
		case ( PikeState::GROW ):
		{
			accelerateGrowth();
			hit_box_.h += grow_speed_;
			if ( hit_box_.h > 30000 )
			{
				hit_box_.h = 30000;
				grow_speed_ = 0;
				state_ = PikeState::SHRINK;
			}
			setVerticalY();
		}
		break;
		case ( PikeState::SHRINK ):
		{
			accelerateGrowth();
			hit_box_.h -= grow_speed_;
			if ( hit_box_.h < 0 )
			{
				hit_box_.h = 0;
				grow_speed_ = 0;
				state_ = PikeState::WAIT;
			}
			setVerticalY();
		}
		break;
	}
};

void VerticalPikeSprite::render( Camera& camera, bool priority )
{
	graphics_->flip_y_ = direction_y_ == Direction::Vertical::DOWN;
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
};

void VerticalPikeSprite::accelerateGrowth()
{
	grow_speed_ += GROW_ACCELERATION;
	if ( grow_speed_ > MAX_GROW_SPEED )
	{
		grow_speed_ = MAX_GROW_SPEED;
	}
};

void VerticalPikeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void VerticalPikeSprite::setVerticalY()
{
	hit_box_.y = ( direction_y_ == Direction::Vertical::DOWN )
		? original_hit_box_.y
		: original_hit_box_.y + Unit::BlocksToSubPixels( 1 ) - hit_box_.h;
};
