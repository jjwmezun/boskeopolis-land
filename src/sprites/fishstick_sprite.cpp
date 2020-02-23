#include "collision.hpp"
#include "fishstick_sprite.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"

FishstickSprite::FishstickSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/fishstick.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 20, 12, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 600, 1400, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING ),
	move_timer_ (),
	pause_timer_ ()
{};

FishstickSprite::~FishstickSprite() {};

void FishstickSprite::customUpdate( LevelState& level_state )
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
			{
				moveLeft();
				graphics_->flip_x_ = false;
			}
			break;
			case ( Direction::Horizontal::RIGHT ):
			{
				moveRight();
				graphics_->flip_x_ = true;
			}
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

void FishstickSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
