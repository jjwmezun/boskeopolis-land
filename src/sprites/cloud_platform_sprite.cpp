#include "collision.hpp"
#include "cloud_platform_sprite.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

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
			0,
			-6,
			0,
			8
		),
		x,
		y,
	   64,
		8,
	   { SpriteType::CLOUD_PLATFORM },
	   100,
	   2400,
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
{
	layer_ = Unit::Layer::SPRITES_2;
};

CloudPlatformSprite::~CloudPlatformSprite() {};

void CloudPlatformSprite::customUpdate( LevelState& level_state )
{
	if ( level_state.sprites().hero().rightSubPixels() < hit_box_.x )
	{
		top_speed_ = 1600;
	}
	else if ( level_state.sprites().hero().hit_box_.x > rightSubPixels() )
	{
		top_speed_ = 3200;
	}
	else
	{
		top_speed_ = 2400;
	}

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
	LevelState& level_state
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
		Map& lvmap = level_state.currentMap();
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
