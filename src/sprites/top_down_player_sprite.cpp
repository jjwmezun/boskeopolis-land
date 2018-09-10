#include "camera.hpp"
#include "input.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "top_down_bullet_sprite.hpp"
#include "top_down_player_sprite.hpp"

TopDownPlayerSprite::TopDownPlayerSprite( int x, int y )
:
	Sprite
	(
		std::make_unique<SpriteGraphics>
		(
			"sprites/autumn_maze.png",
			0,
			0,
			false,
			false,
			0,
			false,
			-2,
			-1,
			3,
			2
		),
		x,
		y,
		12,
		14,
		{},
		500,
		2000,
		0,
		0,
		Direction::Horizontal::__NULL,
		Direction::Vertical::DOWN,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::PERMANENT
	),
	direction_ ( Direction::Simple::DOWN )
{};

TopDownPlayerSprite::~TopDownPlayerSprite() {};

void TopDownPlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_ )
	{
		case( Direction::Simple::UP ):
		{
			testDirectionsList({ Direction::Simple::UP, Direction::Simple::RIGHT, Direction::Simple::LEFT, Direction::Simple::DOWN });
		}
		break;
		case( Direction::Simple::RIGHT ):
		{
			testDirectionsList({ Direction::Simple::RIGHT, Direction::Simple::DOWN, Direction::Simple::UP, Direction::Simple::LEFT });
		}
		break;
		case( Direction::Simple::DOWN ):
		{
			testDirectionsList({ Direction::Simple::DOWN, Direction::Simple::LEFT, Direction::Simple::RIGHT, Direction::Simple::UP });
		}
		break;
		case( Direction::Simple::LEFT ):
		{
			testDirectionsList({ Direction::Simple::LEFT, Direction::Simple::UP, Direction::Simple::DOWN, Direction::Simple::RIGHT });
		}
		break;
	}

	if ( Input::pressed( Input::Action::CAMERA_UP ) )
	{
		shoot( sprites, Direction::Simple::UP );
	}
	else if ( Input::pressed( Input::Action::CAMERA_RIGHT ) )
	{
		shoot( sprites, Direction::Simple::RIGHT );
	}
	else if ( Input::pressed( Input::Action::CAMERA_DOWN ) )
	{
		shoot( sprites, Direction::Simple::DOWN );
	}
	else if ( Input::pressed( Input::Action::CAMERA_LEFT ) )
	{
		shoot( sprites, Direction::Simple::LEFT );
	}
	else if ( Input::pressed( Input::Action::RUN ) )
	{
		shoot( sprites, direction_ );
	}

	camera.adjust( *this, lvmap );
};

void TopDownPlayerSprite::shoot( SpriteSystem& sprites, Direction::Simple direction )
{
	sprites.spawn( std::make_unique<TopDownBulletSprite> ( centerXPixels(), centerYPixels(), direction ) );
};

void TopDownPlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};

void TopDownPlayerSprite::testDirectionsList( std::vector<Direction::Simple>&& directions )
{
	for ( const Direction::Simple dir : directions )
	{
		if ( testForMovement( dir ) )
		{
			return;
		}
	}

	stopX();
	stopY();
}

bool TopDownPlayerSprite::testForMovement( Direction::Simple direction )
{
	switch ( direction )
	{
		case( Direction::Simple::UP ):
		{
			if ( Input::held( Input::Action::MOVE_UP ) )
			{
				direction_ = Direction::Simple::UP;
				moveUp();
				stopX();
				return true;
			}
		}
		break;
		case( Direction::Simple::RIGHT ):
		{
			if ( Input::held( Input::Action::MOVE_RIGHT ) )
			{
				direction_ = Direction::Simple::RIGHT;
				moveRight();
				stopY();
				return true;
			}
		}
		break;
		case( Direction::Simple::DOWN ):
		{
			if ( Input::held( Input::Action::MOVE_DOWN ) )
			{
				direction_ = Direction::Simple::DOWN;
				moveDown();
				stopX();
				return true;
			}
		}
		break;
		case( Direction::Simple::LEFT ):
		{
			if ( Input::held( Input::Action::MOVE_LEFT ) )
			{
				direction_ = Direction::Simple::LEFT;
				moveLeft();
				stopY();
				return true;
			}
		}
		break;
	}

	return false;
}
