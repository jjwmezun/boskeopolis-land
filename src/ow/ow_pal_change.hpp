#pragma once

#include "mezun_sdl2.hpp"

struct OWPalChange
{
	const int type;
	const int x;
	const int y;

	static constexpr int SIZE = 16;

	constexpr OWPalChange( int at, int ax, int ay )
	:
		type ( at ),
		x ( ax ),
		y ( ay )
	{};

	constexpr sdl::rect coords() const
	{
		return
		{
			x,
			y,
			SIZE,
			SIZE
		};
	};
};