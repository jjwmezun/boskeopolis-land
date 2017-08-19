#include "mezun_sdl2.hpp"

std::ostream& operator<<( std::ostream& os, const sdl::color& color )
{
	os
		<< "{ R: " << std::to_string( color.r )
		<< ", B: " << std::to_string( color.b )
		<< ", G: " << std::to_string( color.g )
		<< ", A: " << std::to_string( color.a )
		<< " }";
	return os;
}

namespace sdl
{
	rect& rect::operator=( const rect& c )
	{
		x = c.x;
		y = c.y;
		w = c.w;
		h = c.h;
		return *this;
	};

	rect::rect( rect&& m )
	:
		SDL_Rect( { m.x, m.y, m.w, m.h } )
	{};

	rect& rect::operator=( rect&& m )
	{
		x = m.x;
		y = m.y;
		w = m.w;
		h = m.h;
		return *this;
	};
}