#include "collision.hpp"
#include "health.hpp"
#include "pike_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int MAX_GROW_SPEED = 8000;
static constexpr int GROW_ACCELERATION = 200;
static constexpr int WAIT_TIME = 32;

static constexpr bool verticalIsDownward( Direction::Simple direction )
{
	return ( direction == Direction::Simple::RIGHT || direction == Direction::Simple::DOWN );
};

static constexpr bool horizontalIsLeft( Direction::Simple direction )
{
	return ( direction == Direction::Simple::DOWN || direction == Direction::Simple::LEFT );
};

PikeSprite::PikeSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pike.png", 0, 0, false, false, 0, false, -1, -1, 2, 2 ), x + 3, y + 1, 10, 0, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	grow_speed_ ( 0 ),
	state_ ( PikeState::WAIT ),
	wait_timer_ ( 0 ),
	horizontal_pike_ ( Unit::PixelsToSubPixels( sdl2::SDLRect{ x + 16, y + 3, 0, 10 }) )
{
	direction_ = Direction::Simple::UP;
};

PikeSprite::~PikeSprite() {};

void PikeSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( state_ )
	{
		case ( PikeState::WAIT ):
		{
			if ( wait_timer_ >= WAIT_TIME )
			{
				wait_timer_ = 0;
				state_ = PikeState::GROW;
			}
			else
			{
				++wait_timer_;
			}
		}
		break;
		case ( PikeState::GROW ):
		{
			accelerateGrowth();
			hit_box_.h += grow_speed_;
			horizontal_pike_.w += grow_speed_;
			if ( hit_box_.h > 30000 )
			{
				hit_box_.h = 30000;
				grow_speed_ = 0;
				state_ = PikeState::SHRINK;
			}
			setHorizontalX();
			setVerticalY();
		}
		break;
		case ( PikeState::SHRINK ):
		{
			accelerateGrowth();
			hit_box_.h -= grow_speed_;
			horizontal_pike_.w -= grow_speed_;
			if ( hit_box_.h < 0 )
			{
				hit_box_.h = 0;
				grow_speed_ = 0;
				direction_ = Direction::rotateSimpleClockwise( direction_ );
				state_ = PikeState::WAIT;
			}
			setHorizontalX();
			setVerticalY();
		}
		break;
	}
};

void PikeSprite::render( Camera& camera, bool priority )
{
	graphics_->flip_x_ = false;
	graphics_->flip_y_ = verticalIsDownward( direction_ );
	graphics_->current_frame_x_ = 0;
	graphics_->current_frame_y_ = 0;
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	graphics_->flip_x_ = horizontalIsLeft( direction_ );
	graphics_->flip_y_ = false;
	graphics_->current_frame_x_ = 44 - Unit::SubPixelsToPixels( horizontal_pike_.w );
	graphics_->render( Unit::SubPixelsToPixels( horizontal_pike_ ), &camera, priority );
};

void PikeSprite::accelerateGrowth()
{
	grow_speed_ += GROW_ACCELERATION;
	if ( grow_speed_ > MAX_GROW_SPEED )
	{
		grow_speed_ = MAX_GROW_SPEED;
	}
};

/*
int PikeSprite::getOriginY() const
{
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		{
			return original_hit_box_.y;
		}
		break;
		case ( Direction::Simple::RIGHT ):
		{
			return ;
		}
		break;
		case ( Direction::Simple::UP ):
		{
			return ;
		}
		break;
		case ( Direction::Simple::UP ):
		{
			return ;
		}
		break;
	}
};
*/

void PikeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		const Collision horizontal_collision = them.testCollision( horizontal_pike_ );
		if ( horizontal_collision.collideAny() )
		{
			health.hurt();
		}
	}
};

void PikeSprite::setHorizontalX()
{
	horizontal_pike_.x = horizontalIsLeft( direction_ )
		? original_hit_box_.x - 3000 - horizontal_pike_.w
		: original_hit_box_.x + 11000;
};

void PikeSprite::setVerticalY()
{
	hit_box_.y = verticalIsDownward( direction_ )
		? original_hit_box_.y + 14000
		: original_hit_box_.y - hit_box_.h;
};
