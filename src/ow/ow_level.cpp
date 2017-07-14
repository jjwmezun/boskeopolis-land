#include "animated_graphics.h"
#include "graphics.h"
#include "mezun_math.h"
#include "ow_camera.hpp"
#include "ow_level.hpp"
#include "unit.h"

OWLevel::OWLevel( int type, int x, int y, const AnimatedGraphics& gfx )
:
	type_ ( type ),
	x_ ( x ),
	y_ ( y ),
	gfx_ ( &gfx )
{};

void OWLevel::render( Graphics& graphics, const OWCamera& camera ) const
{
	gfx_->render( graphics, dest( camera ) );
	graphics.renderObject( "tilesets/ow.png", src(), dest( camera ), SDL_FLIP_NONE, 0.0 );
};

sdl2::SDLRect OWLevel::src() const
{
	return
	{
		Unit::BlocksToPixels( mezun::xOfN( type_ + 32, TEXTURE_WIDTH ) ),
		Unit::BlocksToPixels( mezun::yOfN( type_ + 32, TEXTURE_WIDTH ) ),
		SIZE,
		SIZE
	};
};

sdl2::SDLRect OWLevel::dest( const OWCamera& camera ) const
{
	return
	camera.relative( coords() );
};

sdl2::SDLRect OWLevel::coords() const
{
	return
	{
		x_,
		y_,
		SIZE,
		SIZE
	};
};

int OWLevel::lv() const { return type_ + 1; };
int OWLevel::x() const { return x_; };
int OWLevel::y() const { return y_; };