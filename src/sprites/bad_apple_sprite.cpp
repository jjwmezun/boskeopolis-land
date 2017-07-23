#include "collision.h"
#include "bad_apple_graphics.h"
#include "bad_apple_sprite.h"

BadAppleSprite::BadAppleSprite( int x, int y )
:
	Sprite( std::make_unique<BadAppleGraphics> (), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 100, 600, 1000, 1000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{
	direction_x_ = Direction::Horizontal::LEFT;
};

BadAppleSprite::~BadAppleSprite() {};

void BadAppleSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	if ( isDead() )
		block_interact_ = false;

	if ( direction_x_ == Direction::Horizontal::RIGHT )
	{
		moveRight();
	}
	else
	{
		moveLeft();
	}

	if ( collidedRight() )
	{
		direction_x_ = Direction::Horizontal::LEFT;
	}
	if ( collidedLeft() )
	{
		direction_x_ = Direction::Horizontal::RIGHT;
	}
};

void BadAppleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
};
