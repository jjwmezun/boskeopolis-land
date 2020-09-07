#include "neon_overlay.hpp"
#include "render.hpp"

NeonOverlay::NeonOverlay() : hue_ ( 0 ), color_ ( { 255, 0, 0, 255 } ) {};
NeonOverlay::~NeonOverlay() {};

void NeonOverlay::render()
{
	Render::colorCanvasMultiply( color_.r, color_.g, color_.b, 255 );
};

void NeonOverlay::update()
{
	++hue_;
	if ( hue_ > 360 )
	{
		hue_ = 0;
	}
	convertHSVToRGB();
};

//
// I have no idea why this algorithm works the way it does
// or what "f", "p", "q", or "t" are s'posed to stand for.
// All I know is that I got this from some college website,
// https://www.cs.rit.edu/~ncs/color/t_convert.html,
// & that the Wikipedia algorithm sucks
// ( its hue changes were abrupt & not smooth like this 1 ).
//
void NeonOverlay::convertHSVToRGB()
{
	const double VIBRANCE = 1.0;
	const double SATURATION = 1.0;

	const double h = ( double )( hue_ ) / 60.0;
	const int i = ( int )( floor( h ) );
	const double f = h - ( double )( i );
	const double pd = VIBRANCE * ( 1 - SATURATION );
	const double qd = VIBRANCE * ( 1 - SATURATION * f );
	const double td = VIBRANCE * ( 1 - SATURATION * ( 1 - f ) );

	const Uint8 v = ( Uint8 )( VIBRANCE * 255.0 );
	const Uint8 p = ( Uint8 )( pd * 255.0 );
	const Uint8 q = ( Uint8 )( qd * 255.0 );
	const Uint8 t = ( Uint8 )( td * 255.0 );

	switch ( i )
	{
		case ( 0 ):
		{
			color_.r = v;
			color_.g = t;
			color_.b = p;
		}
		break;
		case ( 1 ):
		{
			color_.r = q;
			color_.g = v;
			color_.b = p;
		}
		break;
		case ( 2 ):
		{
			color_.r = p;
			color_.g = v;
			color_.b = t;
		}
		break;
		case ( 3 ):
		{
			color_.r = p;
			color_.g = q;
			color_.b = v;
		}
		break;
		case ( 4 ):
		{
			color_.r = t;
			color_.g = p;
			color_.b = v;
		}
		break;
		case ( 5 ):
		{
			color_.r = v;
			color_.g = p;
			color_.b = q;
		}
		break;
	}
};
