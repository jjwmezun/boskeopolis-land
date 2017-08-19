#include "animated_graphics.hpp"
#include "render.hpp"
#include "mezun_math.hpp"
#include "ow_camera.hpp"
#include "ow_level.hpp"
#include "unit.hpp"

OWLevel::OWLevel( int type, int x, int y, const AnimatedGraphics& gfx )
:
	type_ ( type ),
	x_ ( x ),
	y_ ( y ),
	gfx_ ( &gfx )
{};

void OWLevel::render( const OWCamera& camera ) const
{
	gfx_->render( dest( camera ) );
	Render::renderObject( "tilesets/ow.png", src(), dest( camera ), SDL_FLIP_NONE, 0.0 );
};

sdl::rect OWLevel::src() const
{
	return
	{
		Unit::BlocksToPixels( mezun::xOfN( type_ + 32, TEXTURE_WIDTH ) ),
		Unit::BlocksToPixels( mezun::yOfN( type_ + 32, TEXTURE_WIDTH ) ),
		SIZE,
		SIZE
	};
};

sdl::rect OWLevel::dest( const OWCamera& camera ) const
{
	return
	camera.relative( coords() );
};

sdl::rect OWLevel::coords() const
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