#include "bird_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int HOW_FAR_CAN_SEE_BLOCKS = 12;
static constexpr int HOW_FAR_CAN_SEE_SUBPIXELS = Unit::BlocksToSubPixels( HOW_FAR_CAN_SEE_BLOCKS );
static constexpr int WAIT_LIMIT = 8;
static constexpr int MOVE_DELAY = 16;

BirdSprite::BirdSprite( int x, int y, Direction::Horizontal dir )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bird.png" ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 4000, 0, 0, dir, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	wait_limit_counter_ ( 0 ),
	move_delay_counter_ ( 0 )
{
	graphics_->priority_ = true;
};

BirdSprite::~BirdSprite() {};

void BirdSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( wait_limit_counter_ > WAIT_LIMIT )
	{
		++move_delay_counter_;
	}

	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
			graphics_->flip_x_ = false;
		break;

		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = true;
		break;
	}

	if ( move_delay_counter_ > MOVE_DELAY )
	{
		switch ( direction_x_ )
		{
			case ( Direction::Horizontal::LEFT ):
				moveLeft();
			break;

			case ( Direction::Horizontal::RIGHT ):
				moveRight();
			break;
		}
	}
};

void BirdSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( wait_limit_counter_ <= WAIT_LIMIT && them.hasType( SpriteType::HERO ) )
	{
		if
		(
			them.hit_box_.y < bottomSubPixels()                                  &&
			them.bottomSubPixels() > hit_box_.y                                  &&
			them.hit_box_.x > hit_box_.x - HOW_FAR_CAN_SEE_SUBPIXELS             &&
			them.rightSubPixels() < rightSubPixels() + HOW_FAR_CAN_SEE_SUBPIXELS
		)
		{
			++wait_limit_counter_;
		}
		
		if      ( them.rightSubPixels() < hit_box_.x ) direction_x_ = Direction::Horizontal::LEFT;
		else if ( them.hit_box_.x > rightSubPixels() ) direction_x_ = Direction::Horizontal::RIGHT;
	}
};

void BirdSprite::reset()
{
	resetPosition();
	vx_ = 0;
	acceleration_x_ = 0;
	acceleration_y_ = 0;
	vy_ = 0;
	wait_limit_counter_ = 0;
	move_delay_counter_ = 0;
	is_dead_ = false;
	death_finished_ = false;
	changeMovement( SpriteMovement::Type::FLOATING );
	sprite_interact_ = true;
};