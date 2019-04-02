#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "player_spaceship_sprite.hpp"
#include "shmup_hero_bullet_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

#include <iostream>

PlayerSpaceshipSprite::PlayerSpaceshipSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 0, 0, false, false, 0, false, -1, -1, 2, 2 ), x, y, 14, 14, { SpriteType::HERO }, 500, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	direction_ ( Direction::Simple::RIGHT ),
	camera_direction_ ( Direction::Simple::RIGHT )
{};

PlayerSpaceshipSprite::~PlayerSpaceshipSprite() {};

void PlayerSpaceshipSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	inputMoveAllDirections();
	switch ( camera_direction_ )
	{
		case ( Direction::Simple::UP ):
		{
			if ( !blocksJustAbove( blocks ) )
			{
				hit_box_.y -= 1000;
			}

			if ( yPixels() < camera.boundaryTop() )
			{
				camera.moveUp( std::max( abs( Unit::SubPixelsToPixels( vy_ ) ) + 1, 2 ) );
			}
			else if ( bottomPixels() > camera.boundaryBottom() )
			{
				camera.moveUp( 1 );
			}
			else
			{
				camera.moveUp( 2 );
			}
		}
		break;
		case ( Direction::Simple::RIGHT ):
		{
			if ( !blocksJustRight( blocks ) )
			{
				hit_box_.x += 1000;
			}

			if ( rightPixels() > camera.boundaryRight() )
			{
				camera.moveRight( std::max( abs( Unit::SubPixelsToPixels( vx_ ) ) + 1, 2 ) );
			}
			else if ( xPixels() < camera.boundaryLeft() )
			{
				camera.moveRight( 1 );
			}
			else
			{
				camera.moveRight( 2 );
			}
		}
		break;
		case ( Direction::Simple::DOWN ):
		{
			if ( !blocksJustBelow( blocks ) )
			{
				hit_box_.y += 1000;
			}

			if ( bottomPixels() > camera.boundaryBottom() )
			{
				camera.moveDown( std::max( abs( Unit::SubPixelsToPixels( vy_ ) ) + 1, 2 ) );
			}
			else if ( yPixels() < camera.boundaryTop() )
			{
				camera.moveDown( 1 );
			}
			else
			{
				camera.moveDown( 2 );
			}
		}
		break;
		case ( Direction::Simple::LEFT ):
		{
			if ( !blocksJustLeft( blocks ) )
			{
				hit_box_.x -= 1000;
			}

			if ( xPixels() < camera.boundaryLeft() )
			{
				camera.moveLeft( std::max( abs( Unit::SubPixelsToPixels( vx_ ) ) + 1, 2 ) );
			}
			else if ( rightPixels() > camera.boundaryRight() )
			{
				camera.moveLeft( 1 );
			}
			else
			{
				camera.moveLeft( 2 );
			}
		}
		break;
	}

	if ( hit_box_.x < Unit::PixelsToSubPixels( camera.x() ) )
	{
		hit_box_.x = Unit::PixelsToSubPixels( camera.x() );
	}
	else if ( hit_box_.x + hit_box_.w > Unit::PixelsToSubPixels( camera.right() ) )
	{
		hit_box_.x = Unit::PixelsToSubPixels( camera.right() ) - hit_box_.w;
	}

	containCameraY( camera );

	if ( Input::pressed( Input::Action::RUN ) && Inventory::funds() >= 100 )
	{
		int x = rightPixels();
		int y = centerYPixels();

		switch ( direction_ )
		{
			case ( Direction::Simple::UP ):
			{
				x = centerXPixels();
				y = yPixels();
			}
			break;
			case ( Direction::Simple::DOWN ):
			{
				x = centerXPixels();
				y = bottomPixels();
			}
			break;
			case ( Direction::Simple::LEFT ):
			{
				x = xPixels();
				y = centerYPixels();
			}
			break;
		}

		sprites.spawn( std::make_unique<ShmupHeroBulletSprite> ( x, y, direction_, SpriteType::HEROS_BULLET ) );
		Inventory::loseFunds( 100 );
	}
	invincibilityFlicker( health );

	if ( Input::pressed( Input::Action::CAMERA_UP ) )
	{
		direction_ = Direction::Simple::UP;
	}
	else if ( Input::pressed( Input::Action::CAMERA_RIGHT ) )
	{
		direction_ = Direction::Simple::RIGHT;
	}
	else if ( Input::pressed( Input::Action::CAMERA_DOWN ) )
	{
		direction_ = Direction::Simple::DOWN;
	}
	else if ( Input::pressed( Input::Action::CAMERA_LEFT ) )
	{
		direction_ = Direction::Simple::LEFT;
	}

	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		{
			graphics_->rotation_ = 270.0;
		}
		break;
		case ( Direction::Simple::RIGHT ):
		{
			graphics_->rotation_ = 0.0;
		}
		break;
		case ( Direction::Simple::DOWN ):
		{
			graphics_->rotation_ = 90.0;
		}
		break;
		case ( Direction::Simple::LEFT ):
		{
			graphics_->rotation_ = 180.0;
		}
		break;
	}
};

void PlayerSpaceshipSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::ENEMY ) && my_collision.collideAny() )
	{
		health.hurt();
	}
	else if ( them.hasType( SpriteType::CAMERA_MOVE ) )
	{
		if ( them.top_speed_ != 0 )
		{
			if ( them.direction_y_ != Direction::Vertical::__NULL )
			{
				camera_direction_ = Direction::verticalToSimple( them.direction_y_ );
				them.kill();
			}
			else if ( them.direction_x_ != Direction::Horizontal::__NULL )
			{
				camera_direction_ = Direction::horizontalToSimple( them.direction_x_ );
				them.kill();
			}
		}
		else
		{
			them.top_speed_ = ( int )( camera_direction_ );
		}
	}
};
