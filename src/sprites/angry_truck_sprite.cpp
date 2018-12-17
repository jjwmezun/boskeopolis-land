#include "angry_truck_sprite.hpp"
#include "block_system.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

AngryTruckSprite::AngryTruckSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/angry_truck.png" ), x, y, 40, 32, {}, 2000, 2000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	parked_ ( false )
{};

AngryTruckSprite::~AngryTruckSprite() {};

void AngryTruckSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( parked_ )
	{
		fullStopX();
	}
	else
	{
		turnOnCollide();
		moveInDirectionX();
		graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
	}

	if ( fellInBottomlessPit( lvmap ) )
	{
		kill();
	}
};

void AngryTruckSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::TRUCK_PLATFORM ) )
	{
		if ( hit_box_.x > them.hit_box_.x + 16000 )
		{
			parked_ = true;
			them.jump_lock_ = false;
		}
	}
	else
	{
		if ( their_collision.collideAny() )
		{
			if ( their_collision.collideBottom() )
			{
				if
				(
					!blocks.blocksInTheWay
					(
						{
							them.rightSubPixels(),
							them.topSubPixels(),
							1000,
							them.heightSubPixels()
						},
						BlockComponent::Type::SOLID
					)
				)
				{
					them.hit_box_.x += vx_;
				}
				them.collideStopAny( their_collision );
			}

			if ( them.hasType( SpriteType::HERO ) && !their_collision.collideBottom() )
			{
				health.hurt();
			}
		}
	}
};
