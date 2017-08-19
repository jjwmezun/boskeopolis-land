#pragma once

class Camera;
class Palette;

#include "mezun_sdl2.hpp"
#include <SDL2/SDL.h>
#include <map>
#include <vector>

namespace Render
{
	void init( const std::vector<std::string>& args );
	void quit();

	void renderRect( const sdl::rect& box, int color = 1, int alpha = 255 );
	void renderObject
	(
		const std::string& sheet,
		const sdl::rect& source,
		const sdl::rect& dest,
		bool flip_x,
		bool flip_y,
		double rotation,
		Uint8 alpha,
		const Camera* camera
	);
	void renderObject
	(
		const std::string& orig_sheet,
		sdl::rect source,
		sdl::rect dest,
		SDL_RendererFlip flip = SDL_FLIP_NONE,
		double rotation = 0,
		Uint8 alpha = 255,
		const Camera* = nullptr
	);

	void colorCanvas();
	void clearScreen();
	void screenBorders();
	void presentScreen();

	void newPalette( Palette palette );

	void clearSurfaces();
	void loadTexture( const std::string& sheet, Uint8 alpha = 255 );
};