#include "angry_truck_sprite.hpp"
#include "block_system.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

AngryTruckSprite::AngryTruckSprite( int x, int y, bool permanent, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/angry_truck.png" ), x, y, 40, 32, {}, 2000, 2000, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, ( ( permanent ) ? CameraMovement::PERMANENT : CameraMovement::RESET_OFFSCREEN_AND_AWAY ) ),
	parked_ ( false )
{};

AngryTruckSprite::~AngryTruckSprite() {};

void AngryTruckSprite::customUpdate( LevelState& level_state )
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

	if ( fellInBottomlessPit( level_state.currentMap() ) )
	{
		killNoAnimation();
	}
};

void AngryTruckSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const Collision collision = them.testBlockCollision( *this );
	if ( them.hasType( SpriteType::TRUCK_PLATFORM ) && collision.collideAny() )
	{
		if ( hit_box_.x > them.hit_box_.x + 16000 )
		{
			parked_ = true;
			them.jump_lock_ = false;
		}
	}
	else
	{
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( collision.collideBottom() )
			{
				if
				(
					!level_state.blocks().blocksInTheWay
					(
						{
							them.rightSubPixels(),
							them.topSubPixels(),
							1000,
							them.heightSubPixels()
						},
						BlockComponent::Type::SOLID
					) &&
					!level_state.blocks().blocksInTheWay
					(
						{
							them.hit_box_.x - 1000,
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
				them.collideStopAny( collision );
			}
			else if ( collision.collideAny() )
			{
				level_state.health().hurt();
			}
		}
	}
};

void AngryTruckSprite::reset()
{
	hit_box_ = original_hit_box_;
	parked_ = false;
};
