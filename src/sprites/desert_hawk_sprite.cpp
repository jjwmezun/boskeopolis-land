#include "block_system.hpp"
#include "collision.hpp"
#include "desert_hawk_sprite.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int FRAME_SPEED = 16;
static int setDesertHawkGraphics( int frame )
{
	switch ( frame )
	{
		case ( 0 ): return 0; break;
		case ( 2 ): return 48; break;
		default: return 24;
	}
}

DesertHawkSprite::DesertHawkSprite( int x, int y, Direction::Horizontal direction, std::unique_ptr<SpriteComponent>&& component, CameraMovement camera_movement )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/desert-hawk.png", 0, 0, false, false, 0, 0, -5, 0, 5 ), x, y, 24, 11, {}, 700, 1400, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, camera_movement, true, false ),
	animation_frame_ (),
	animation_timer_ (),
	component_ ( std::move( component ) )
{
	layer_ = Unit::Layer::SPRITES_2;
};

DesertHawkSprite::~DesertHawkSprite() {};

void DesertHawkSprite::customUpdate( LevelState& level_state )
{
	// Behavior
	if ( component_ != nullptr )
	{
		component_->update( *this, level_state );
	}
	else
	{
		moveInDirectionX();
	}

	// Graphics
	if ( animation_timer_.update() )
	{
		graphics_->current_frame_x_ = setDesertHawkGraphics( ( ++animation_frame_ )() );
	}
	flipGraphicsOnRight();
};

void DesertHawkSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		const Collision collision = them.testBlockCollision( *this );
		if ( collision.collideBottom() )
		{
			BlockSystem& blocks = level_state.blocks();
			if
			(
				(
					direction_x_ == Direction::Horizontal::RIGHT &&
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
				) ||
				(
					direction_x_ == Direction::Horizontal::LEFT &&
					!blocks.blocksInTheWay
					(
						{
							them.leftSubPixels() - 1000,
							them.topSubPixels(),
							1000,
							them.heightSubPixels()
						},
						BlockComponent::Type::SOLID
					)
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
};

void DesertHawkSprite::reset()
{
	resetPosition();
	if ( component_ != nullptr )
	{
		component_->reset();
	}
};
