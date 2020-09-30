#include "collision.hpp"
#include "level_state.hpp"
#include "horizontal_pike_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int MAX_GROW_SPEED = 16000;
static constexpr int GROW_ACCELERATION = 400;
static constexpr int WAIT_TIME = 8;

HorizontalPikeSprite::HorizontalPikeSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pike.png", 12, 12, false, false, 0, -1, -1, 2, 2 ), x, y, 0, 10, { SpriteType::ENEMY }, 0, 0, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	grow_speed_ ( 0 ),
	state_ ( PikeState::WAIT ),
	wait_timer_ ( 0 )
{};

HorizontalPikeSprite::~HorizontalPikeSprite() {};

void HorizontalPikeSprite::customUpdate( LevelState& level_state )
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
			hit_box_.w += grow_speed_;
			if ( hit_box_.w > 78000 )
			{
				hit_box_.w = 78000;
				grow_speed_ = 0;
				state_ = PikeState::SHRINK;
			}
			setHorizontalX();
		}
		break;
		case ( PikeState::SHRINK ):
		{
			accelerateGrowth();
			hit_box_.w -= grow_speed_;
			if ( hit_box_.w < 0 )
			{
				hit_box_.w = 0;
				grow_speed_ = 0;
				state_ = PikeState::WAIT;
			}
			setHorizontalX();
		}
		break;
	}
};

void HorizontalPikeSprite::render( const Camera& camera ) const
{
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::LEFT;
	graphics_->current_frame_x_ = 92 - Unit::SubPixelsToPixels( hit_box_.w );
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
};

void HorizontalPikeSprite::accelerateGrowth()
{
	grow_speed_ += GROW_ACCELERATION;
	if ( grow_speed_ > MAX_GROW_SPEED )
	{
		grow_speed_ = MAX_GROW_SPEED;
	}
};

void HorizontalPikeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void HorizontalPikeSprite::setHorizontalX()
{
	hit_box_.x = ( direction_x_ == Direction::Horizontal::LEFT )
		? original_hit_box_.x + Unit::BlocksToSubPixels( 1 ) - hit_box_.w
		: original_hit_box_.x;
};
