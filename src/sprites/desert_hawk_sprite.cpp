#include "collision.hpp"
#include "desert_hawk_sprite.hpp"
#include "health.hpp"
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
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/desert-hawk.png", 0, 0, false, false, 0, true, 0, -5, 0, 5 ), x, y, 24, 11, {}, 700, 1400, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, false )
{
	if ( direction == Direction::Horizontal::RIGHT )
	{
		graphics_->flip_x_ = true;
	}
};

DesertHawkSprite::~DesertHawkSprite() {};

void DesertHawkSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
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

void DesertHawkSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideBottom() )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			them.hit_box_.x += vx_;
			if ( them.hit_box_.x < 0 )
			{
				them.hit_box_.x = 0;
			}
		}
		else if ( their_collision.collideAny() )
		{
			health.hurt();
		}
	}
};
