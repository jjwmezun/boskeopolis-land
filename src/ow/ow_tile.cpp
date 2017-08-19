#include "animated_graphics.hpp"
#include "render.hpp"
#include "mezun_math.hpp"
#include "ow_camera.hpp"
#include "ow_hero.hpp"
#include "ow_tile.hpp"
#include "unit.hpp"

OWTile::OWTile( int type, int x, int y, const AnimatedGraphics& gfx )
:
	type_ ( type ),
	x_ ( x ),
	y_ ( y ),
	gfx_ ( nullptr )
{
	setGFX( type, gfx );
};

void OWTile::render( const OWCamera& camera ) const
{
	if ( gfx_ != nullptr )
	{
		gfx_->render( dest( camera ) );
	}
	else
	{
		Render::renderObject( "tilesets/ow.png", src(), dest( camera ), SDL_FLIP_NONE, 0.0 );
	}
};

void OWTile::highlight( const OWCamera& camera ) const
{
	Render::renderRect( dest( camera ), 4, 128 );
};

sdl::rect OWTile::src() const
{
	return
	{
		Unit::BlocksToPixels( mezun::xOfN( type_, TEXTURE_WIDTH ) ),
		Unit::BlocksToPixels( mezun::yOfN( type_, TEXTURE_WIDTH ) ),
		SIZE,
		SIZE
	};
};

sdl::rect OWTile::dest( const OWCamera& camera ) const
{
	return camera.relative( coords() );
};

void OWTile::interact( OWHero& hero, const Collision& collision ) const
{
	switch ( type_ )
	{
		case ( 1 ):
		case ( 2 ):
		case ( 3 ):
		case ( 6 ):
		case ( 8 ):
		case ( 10 ):
		case ( 21 ):
		case ( 22 ):
			hero.collideStop( collision );
		break;
	}
};

sdl::rect OWTile::coords() const
{
	return
	{
		x_,
		y_,
		SIZE,
		SIZE
	};
};

void OWTile::changeType( int type, const SpriteGraphics& gfx )
{
	type_ = type;
	setGFX( type, gfx );
};

void OWTile::setGFX( int type, const SpriteGraphics& gfx )
{
	if ( type == 1 )
	{
		gfx_ = &gfx;
	}
	else
	{
		gfx_ = nullptr;
	}
};