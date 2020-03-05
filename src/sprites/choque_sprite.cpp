#include "choque_sprite.hpp"
#include "collision.hpp"
#include "direction.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"

static constexpr int EYE_LENGTH = 2;
static constexpr int DEFAULT_LEFT_X = 7;
static constexpr int DEFAULT_LEFT_Y = 16;
static constexpr int DEFAULT_RIGHT_X = 15;
static constexpr int DEFAULT_RIGHT_Y = 7;

struct EyePosition
{
	int left_x;
	int left_y;
	int right_x;
	int right_y;
};
EyePosition getEyePosition( Direction::Rotation r );

ChoqueSprite::ChoqueSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/choque.png" ), x, y, 24, 24, {}, 100, 1500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	src_ ( 1, 21, EYE_LENGTH, EYE_LENGTH ),
	dest_eye_left_ ( x + DEFAULT_LEFT_X, y + DEFAULT_LEFT_Y, EYE_LENGTH, EYE_LENGTH ),
	dest_eye_right_ ( x + DEFAULT_RIGHT_X, y + DEFAULT_RIGHT_Y, EYE_LENGTH, EYE_LENGTH ),
	eye_placement_ ( Direction::Rotation::__NULL ),
	still_ ( true )
{
	fall_start_speed_ = gravity_start_speed_ * 1.5;
	fall_top_speed_ = gravity_top_speed_ * 1.5;
};

ChoqueSprite::~ChoqueSprite() {};

void ChoqueSprite::customUpdate( LevelState& level_state )
{
	if ( collide_top_ )
	{
		changeMovement( SpriteMovement::Type::FLOATING );
	}
	
	if ( hasMovementType( SpriteMovement::Type::FLOATING ) && !still_ )
	{
		moveUp();
	}

	if ( collide_bottom_ )
	{
		still_ = true;
	}
};

void ChoqueSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if
		(
			them.hit_box_.x > hit_box_.x - Unit::BlocksToSubPixels( 2 ) &&
			them.rightSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 2 )
		)
		{
			if ( still_ )
			{
				changeMovement( SpriteMovement::Type::GROUNDED );
				still_ = false;
			}

			Direction::Horizontal relative_x = Direction::Horizontal::__NULL;
			Direction::Vertical   relative_y = Direction::Vertical::__NULL;

			if ( them.centerXSubPixels() < centerXSubPixels() - 10000 )
			{
				relative_x = Direction::Horizontal::LEFT;
			}
			else if ( them.centerXSubPixels() > centerXSubPixels() + 10000 )
			{
				relative_x = Direction::Horizontal::RIGHT;
			}

			if ( them.centerYSubPixels() < centerYSubPixels() - 10000 )
			{
				relative_y = Direction::Vertical::UP;
			}
			else if ( them.centerYSubPixels() > centerYSubPixels() + 10000 )
			{
				relative_y = Direction::Vertical::DOWN;
			}
			eye_placement_ = Direction::horizontalAndVerticalToRotation( relative_x, relative_y );
		}
		else
		{
			eye_placement_ = Direction::Rotation::__NULL;
		}
	}

	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );

		if ( their_collision.collideBottom() )
		{
			them.hit_box_.y += vy_;
		}
		
		if
		(
			them.hasType( SpriteType::HERO ) &&
			( ( their_collision.collideBottom() && them.collide_bottom_ ) ||
			( their_collision.collideTop() && them.collide_top_ ) )
		)
		{
			them.kill();
		}
	}
};

void ChoqueSprite::reset()
{
	fullStopY();
	resetPosition();
	changeMovement( SpriteMovement::Type::FLOATING );
	still_ = true;
};

void ChoqueSprite::render( Camera& camera, bool priority )
{
	// Setup Data
	const EyePosition position = getEyePosition( eye_placement_ );
	dest_eye_left_.x = position.left_x + xPixels();
	dest_eye_left_.y = position.left_y + yPixels();
	dest_eye_right_.x = position.right_x + xPixels();
	dest_eye_right_.y = position.right_y + yPixels();

	// Render
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
	Render::renderObject( graphics_->texture_, src_, dest_eye_left_, SDL_FLIP_NONE, 0.0, 255, &camera );
	Render::renderObject( graphics_->texture_, src_, dest_eye_right_, SDL_FLIP_NONE, 0.0, 255, &camera );
};

EyePosition getEyePosition( Direction::Rotation r )
{
	switch ( r )
	{
		case( Direction::Rotation::UP ):         return { 6, 14, 16, 5 };
		case( Direction::Rotation::UP_RIGHT ):   return { 7, 14, 17, 5 };
		case( Direction::Rotation::RIGHT ):      return { 7, 15, 17, 6 };
		case( Direction::Rotation::DOWN_RIGHT ): return { 7, 16, 17, 7 };
		case( Direction::Rotation::DOWN ):       return { 6, 16, 16, 7 };
		case( Direction::Rotation::DOWN_LEFT ):  return { 5, 16, 15, 7 };
		case( Direction::Rotation::LEFT ):       return { 5, 15, 15, 6 };
		case( Direction::Rotation::UP_LEFT ):    return { 5, 14, 15, 5 };
		case( Direction::Rotation::__NULL ):
			return
			{
				DEFAULT_LEFT_X,
				DEFAULT_LEFT_Y,
				DEFAULT_RIGHT_X,
				DEFAULT_RIGHT_Y
			};
	}
};