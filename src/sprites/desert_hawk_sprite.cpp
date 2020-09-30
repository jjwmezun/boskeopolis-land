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
		case ( 0 ):
			return 0;
		break;
		case ( 2 ):
			return 48;
		break;
		default:
			return 24;
		break;
	}
}

DesertHawkSprite::DesertHawkSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/desert-hawk.png", 0, 0, false, false, 0, 0, -5, 0, 5 ), x, y, 24, 11, {}, 700, 1400, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, false )
{
	layer_ = Unit::Layer::SPRITES_2;
	if ( direction == Direction::Horizontal::RIGHT )
	{
		graphics_->flip_x_ = true;
	}
};

DesertHawkSprite::~DesertHawkSprite() {};

void DesertHawkSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();

	if ( timer_ >= FRAME_SPEED - 1 )
	{
		++frame_;
		if ( frame_ >= 4 )
		{
			frame_ = 0;
		}
		graphics_->current_frame_x_ = setDesertHawkGraphics( frame_ );

		timer_ = 0;
	}
	else
	{
		++timer_;
	}
};

void DesertHawkSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideBottom() )
		{
			BlockSystem& blocks = level_state.blocks();
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
				) &&
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
			{
				them.hit_box_.x += vx_;
			}
			them.collideStopAny( their_collision );
		}
		else if ( their_collision.collideAny() )
		{
			level_state.health().hurt();
		}
	}
};
