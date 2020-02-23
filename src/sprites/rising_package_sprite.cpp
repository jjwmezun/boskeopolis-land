#include "rising_package_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

RisingPackageSprite::RisingPackageSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/rising-package.png" ), x, y, 48, 192, {}, 1000, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	rise_ ( false )
{};

RisingPackageSprite::~RisingPackageSprite() {};

void RisingPackageSprite::customUpdate( LevelState& level_state )
{
	if ( rise_ )
	{
		moveUp();
	}
};

void RisingPackageSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		if ( their_collision.collideBottom() )
		{
			rise_ = true;
		}
	}
};

void RisingPackageSprite::reset()
{
	resetPosition();
	rise_ = false;
	fullStopY();
};
