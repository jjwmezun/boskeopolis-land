#include "collision.hpp"
#include "cloud_platform_sprite.hpp"
#include "map.hpp"
#include "sprite_graphics.hpp"

CloudPlatformSprite::CloudPlatformSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite
	(
		std::make_unique<SpriteGraphics>
		(
			"sprites/cloud_platform.png",
			0,
			0,
			false,
			false,
			0,
			true,
			0,
			-6,
			0,
			10
		),
		x,
		y,
	   64,
		8,
	   { SpriteType::CLOUD_PLATFORM },
	   100,
	   1800,
	   0,
	   0,
	   direction,
	   Direction::Vertical::__NULL,
	   nullptr,
	   SpriteMovement::Type::FLOATING,
	   CameraMovement::PERMANENT,
	   false,
	   false
	),
	started_ ( false )
{};

CloudPlatformSprite::~CloudPlatformSprite() {};

void CloudPlatformSprite::customUpdate
(
	Camera& camera,
	Map& lvmap,
	EventSystem& events,
	SpriteSystem& sprites,
	BlockSystem& blocks
)
{
	switch( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
			moveLeft();
		break;
		case ( Direction::Horizontal::RIGHT ):
			moveRight();
		break;
		case ( Direction::Horizontal::__NULL ):
			fullStopX();
		break;
	}
};

void CloudPlatformSprite::customInteract
(
	Collision& my_collision,
	Collision& their_collision,
	Sprite& them,
	BlockSystem& blocks,
	SpriteSystem& sprites,
	Map& lvmap
)
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideBottom() )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			them.addToX( vx_ );
			them.addToY( vy_ );
			started_ = true;
		}

		
		// Ensure cloud platform comes back if it leaves you 'hind on a different platform.
		switch ( direction_x_ )
		{
			case ( Direction::Horizontal::LEFT ):
				if ( xPixels() < them.xPixels() - ( lvmap.scrollLoopWidthPixels() / 2 ) )
				{
					hit_box_.x = them.xSubPixels() + Unit::PixelsToSubPixels( ( lvmap.scrollLoopWidthPixels() / 2 ) );
				}
			break;

			case ( Direction::Horizontal::RIGHT ):
				if ( xPixels() > them.xPixels() + ( lvmap.scrollLoopWidthPixels() / 2 ) )
				{
					hit_box_.x = them.xSubPixels() - Unit::PixelsToSubPixels( ( lvmap.scrollLoopWidthPixels() / 2 ) );
				}
			break;
		}
	}
};
