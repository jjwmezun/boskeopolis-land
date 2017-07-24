#pragma once

class Palette;

#include "mezun_sdl2.hpp"
#include <SDL2/SDL.h>
#include <map>
#include <vector>

namespace Render
{
	void init( const std::vector<std::string>& args );
	void quit();

	void renderRect( const sdl2::SDLRect& box, int color = 1, int alpha = 255 );
	void renderObject
	(
		const std::string& sheet,
		sdl2::SDLRect source,
		sdl2::SDLRect dest,
		SDL_RendererFlip flip = SDL_FLIP_NONE,
		double rotation = 0,
		Uint8 alpha = 255
	);

	void colorCanvas();
	void clearScreen();
	void screenBorders();
	void presentScreen();

	void newPalette( Palette palette );

	void clearSurfaces();
	void loadTexture( const std::string& sheet, Uint8 alpha = 255 );
};