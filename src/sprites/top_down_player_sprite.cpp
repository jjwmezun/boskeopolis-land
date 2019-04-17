#include "camera.hpp"
#include "input.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "top_down_bullet_sprite.hpp"
#include "top_down_player_sprite.hpp"

#include <iostream>

constexpr int FRAME_LENGTH = 8;

constexpr int getDirectionGraphics( Direction::Simple direction )
{
	switch ( direction )
	{
		case( Direction::Simple::UP ):
		{
			return 16;
		}
		break;

		case( Direction::Simple::RIGHT ):
		{
			return 48;
		}
		break;

		case( Direction::Simple::LEFT ):
		{
			return 32;
		}
		break;

		default:
		{
			return 0;
		}
		break;
	}
}

TopDownPlayerSprite::TopDownPlayerSprite( int x, int y )
:
	Sprite
	(
		std::make_unique<SpriteGraphics>
		(
			"sprites/autumn-dungeon.png",
			0,
			0,
			false,
			false,
			0,
			true,
			-2,
			-1,
			3,
			2
		),
		x,
		y,
		12,
		14,
		{ SpriteType::HERO },
		500,
		3000,
		0,
		0,
		Direction::Horizontal::__NULL,
		Direction::Vertical::DOWN,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::PERMANENT
	),
	direction_ ( Direction::Simple::DOWN ),
	animation_timer_ ( 0 ),
	shoot_animation_timer_ ( -1 ),
	is_shooting_ ( false )
{};

TopDownPlayerSprite::~TopDownPlayerSprite() {};

void TopDownPlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	handleMovement();
	handleShooting( sprites );
	camera.adjust( *this, lvmap );
	updateGraphics();
};

void TopDownPlayerSprite::handleMovement()
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
}

void TopDownPlayerSprite::handleShooting( SpriteSystem& sprites )
{
	is_shooting_ = false;
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
}

void TopDownPlayerSprite::shoot( SpriteSystem& sprites, Direction::Simple direction )
{
	int x = centerXPixels();
	int y = centerYPixels();
	switch ( direction )
	{
		case( Direction::Simple::UP ):
		{
			y = yPixels();
		}
		break;
		case( Direction::Simple::RIGHT ):
		{
			x = rightPixels();
		}
		break;
		case( Direction::Simple::DOWN ):
		{
			y = bottomPixels();
		}
		break;
		case( Direction::Simple::LEFT ):
		{
			x = xPixels();
		}
		break;
	}
	sprites.spawn( std::make_unique<TopDownBulletSprite> ( x, y, direction ) );
	is_shooting_ = true;
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

void TopDownPlayerSprite::updateGraphics()
{
	graphics_->current_frame_y_ = getDirectionGraphics( direction_ );

	int shoot_animation_adjust = 0;
	if ( is_shooting_ )
	{
		shoot_animation_timer_ = 0;
	}

	if ( shoot_animation_timer_ > -1 )
	{
		shoot_animation_adjust = 30;
		if ( shoot_animation_timer_ >= FRAME_LENGTH )
		{
			shoot_animation_timer_ = -1;
		}
		else
		{
			++shoot_animation_timer_;
		}
	}

	if ( is_moving_ )
	{
		if ( animation_timer_ >= FRAME_LENGTH )
		{
			graphics_->current_frame_x_ = ( ( graphics_->current_frame_x_ == 0 ) ? 15 : 0 ) + shoot_animation_adjust;
			animation_timer_ = 0;
		}
		else
		{
			++animation_timer_;
		}
	}
	else
	{
		graphics_->current_frame_x_ = shoot_animation_adjust;
		animation_timer_ = 0;
	}
}
