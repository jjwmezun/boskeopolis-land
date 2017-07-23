#include "block_component.hpp"
#include "block_system.hpp"
#include "collision.hpp"
#include <iostream>
#include "sillyfish_sprite.hpp"
#include "sillyfish_graphics.hpp"

SillyfishSprite::SillyfishSprite( int x, int y )
:
	Sprite( std::make_unique<SillyfishGraphics> (), x, y, 16, 32, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	start_ground_lock_ ( false ),
	switch_lock_ ( { 4, false } )
{};

SillyfishSprite::~SillyfishSprite() {};

void SillyfishSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	switch ( directionX() )
	{
		case ( Direction::Horizontal::LEFT ):

			if( !blocks.blocksInTheWay
			(
				{
					leftSubPixels() - Unit::BlocksToSubPixels( 1 ),
					bottomSubPixels(),
					Unit::BlocksToSubPixels( 1 ),
					Unit::BlocksToSubPixels( 1 )

				},
				BlockComponent::Type::SOLID 
			))
			{
				direction_x_ = Direction::Horizontal::RIGHT;
			}

			if ( collidedLeft() )
			{
				direction_x_ = Direction::Horizontal::RIGHT;
			}

		break;

		case ( Direction::Horizontal::RIGHT ):

			if( !blocks.blocksInTheWay
			(
				{
					rightSubPixels(),
					bottomSubPixels(),
					Unit::BlocksToSubPixels( 1 ),
					Unit::BlocksToSubPixels( 1 )

				},
				BlockComponent::Type::SOLID 
			))
			{
				direction_x_ = Direction::Horizontal::LEFT;
			}

			if ( collidedRight() )
			{
				direction_x_ = Direction::Horizontal::LEFT;
			}

		break;

		case ( Direction::Horizontal::__NULL ):
			std::cout<<"ERROR: Sillyfish has invalid direction."<<std::endl;
		break;
	}

	switch ( directionX() )
	{
		case ( Direction::Horizontal::LEFT ):

			moveLeft();

		break;

		case ( Direction::Horizontal::RIGHT ):

			moveRight();

		break;

		case ( Direction::Horizontal::__NULL ):
			std::cout<<"ERROR: Sillyfish has invalid direction."<<std::endl;
		break;
	}

};

void SillyfishSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
};
