#include "collision.hpp"
#include "ice_box_rock_solid_hack_sprite.hpp"
#include "main.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"

IceBoxRockSolidHackSprite::IceBoxRockSolidHackSprite( int x, int y )
:
	Sprite( nullptr, x, y, 12 * 16, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	extra_solids_
	(
		Unit::PixelsToSubPixels( sdl2::SDLRect { x + 240, y + 32,  64, 64 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect { x + 80,  y + 96, 320,  32 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect { x, y + 32, 32, 160 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect { x, y + 192, 240, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect { x + 272, y + 192, 224, 16 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect { x + 448, y + 128, 32, 64 } ),
		Unit::PixelsToSubPixels( sdl2::SDLRect { x + 352, y, 128, 32 } )
	)
{};

IceBoxRockSolidHackSprite::~IceBoxRockSolidHackSprite() {};

void IceBoxRockSolidHackSprite::customUpdate( LevelState& level_state )
{};

void IceBoxRockSolidHackSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( !them.hasType( SpriteType::ICICLE ) )
	{
		if ( their_collision.collideAny() )
		{
			them.collideStopAny( their_collision );
		}
		for ( int i = 0; i < NUMBER_OF_EXTRA_SOLIDS; ++i )
		{
			const auto collision = them.movement_->testCollision( them, extra_solids_[ i ] );
			them.collideStopAny( collision );
		}
	}
};
