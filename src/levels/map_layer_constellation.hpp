#pragma once

#include "mezun_sdl2.hpp"
#include <string>

namespace MapLayerConstellation
{
	enum class StarType
	{
		NO_STAR = -1,
		SMALL_STAR = 0,
		MEDIUM_STAR = 1,
		BIG_STAR = 2,
		BRIGHT_SMALL_STAR = 3,
		BRIGHT_MEDIUM_STAR = 4,
		BRIGHT_BIG_STAR = 5
	};
	std::string graphicsPath();
	SDL_Texture* formTexture( const sdl2::SDLRect& src );
};