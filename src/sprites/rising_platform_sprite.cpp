#include "rising_platform_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int MOVEMENT_AMOUNT = Unit::BlocksToSubPixels( 8 );

RisingPlatformSprite::RisingPlatformSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/weight-platform.png" ), x, y, 48, 8, {}, 200, 1600, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::UP, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

RisingPlatformSprite::~RisingPlatformSprite() {};

void RisingPlatformSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_y_ )
	{
		case ( Direction::Vertical::UP ):
		{
			moveUp();
			if ( hit_box_.y <= original_hit_box_.y - MOVEMENT_AMOUNT )
			{
				direction_y_ = Direction::Vertical::DOWN;
			}
		}
		break;

		case ( Direction::Vertical::DOWN ):
		{
			moveDown();
			if ( hit_box_.y >= original_hit_box_.y )
			{
				direction_y_ = Direction::Vertical::UP;
			}
		}
		break;
	}
};

void RisingPlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		if ( their_collision.collideTop() )
		{
			them.hit_box_.y += vy_;
		}
	}
};
