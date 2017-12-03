#include "collision.hpp"
#include "bad_apple_graphics.hpp"
#include "bad_apple_sprite.hpp"

BadAppleSprite::BadAppleSprite( int x, int y, Direction::Horizontal dir_x )
:
	Sprite( std::make_unique<BadAppleGraphics> (), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 100, 600, 1000, 1000, dir_x, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

BadAppleSprite::~BadAppleSprite() {};

void BadAppleSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
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

void BadAppleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};