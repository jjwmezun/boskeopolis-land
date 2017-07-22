#include "collision.h"
#include "fishstick_sprite.h"
#include "sprite_graphics.h"

FishstickSprite::FishstickSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/fishstick.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 20, 12, { SpriteType::ENEMY }, 600, 1400, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING ),
	move_timer_ ( { 16, true } ),
	pause_timer_ ( { 8, false } )
{};

FishstickSprite::~FishstickSprite() {};

void FishstickSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	if ( move_timer_.hit() )
	{
		pause_timer_.start();
		move_timer_.stop();
	}
	else if ( pause_timer_.hit() )
	{
		move_timer_.start();
		direction_x_ = Direction::randomHorizontal();
		pause_timer_.stop();
	}

	if ( move_timer_.on() )
	{
		move_timer_.update();

		switch( directionX() )
		{
			case ( Direction::Horizontal::LEFT ):
				moveLeft();
				graphics_->flip_x_ = false;
			break;
			case ( Direction::Horizontal::RIGHT ):
				moveRight();
				graphics_->flip_x_ = true;
			break;
			case ( Direction::Horizontal::__NULL ):
				// Do Nothing.
			break;
		}
	}
	else
	{
		pause_timer_.update();
		stopX();
		vx_ /= 1.2;
	}
};

void FishstickSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.hurt();
	}
};