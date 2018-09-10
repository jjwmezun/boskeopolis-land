#include "simple_camera.hpp"

bool SimpleCamera::onscreen( const sdl2::SDLRect& r, int padding ) const
{
	return !offscreen( r, padding );
};

bool SimpleCamera::onscreenPixels( const sdl2::SDLRect& r, int padding ) const
{
	sdl2::SDLRect new_r = Unit::PixelsToSubPixels( r );
	return !offscreen( new_r, padding );
};

bool SimpleCamera::offscreen( const sdl2::SDLRect& r, int padding, Direction::Simple dir ) const
{
	switch( dir )
	{
		case ( Direction::Simple::UP ):
			return offscreenTop( r, padding );
		break;
		case ( Direction::Simple::RIGHT ):
			return offscreenRight( r, padding );
		break;
		case ( Direction::Simple::DOWN ):
			return offscreenBottom( r, padding );
		break;
		case ( Direction::Simple::LEFT ):
			return offscreenLeft( r, padding );
		break;
		default:
			return offscreenTop( r, padding ) || offscreenRight( r, padding ) || offscreenBottom( r, padding ) || offscreenLeft( r, padding );
		break;
	}
};
