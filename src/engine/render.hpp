#pragma once

class Camera;
class Palette;

#include "mezun_sdl2.hpp"
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include "unit.hpp"

namespace Render
{
	static constexpr sdl2::SDLRect window_box_ = { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS };
	void init( const std::vector<std::string>& args );
	void quit();

	void renderRect( const sdl2::SDLRect& box, int color = 1, int alpha = 255 );
	void renderRectCamera( sdl2::SDLRect& box, const Camera& camera, int color, int alpha );
	void renderObject
	(
		const std::string& sheet,
		const sdl2::SDLRect& source,
		const sdl2::SDLRect& dest,
		bool flip_x,
		bool flip_y,
		double rotation,
		Uint8 alpha,
		const Camera* camera,
		SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE,
		SDL_Texture* alt_texture = nullptr
	);
	void renderObject
	(
		const std::string& sheet,
		sdl2::SDLRect source,
		sdl2::SDLRect dest,
		SDL_RendererFlip flip = SDL_FLIP_NONE,
		double rotation = 0,
		Uint8 alpha = 255,
		const Camera* = nullptr,
		SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE,
		SDL_Texture* alt_texture = nullptr
	);
	void renderObject
	(
		SDL_Texture* texture,
		sdl2::SDLRect source,
		sdl2::SDLRect dest,
		SDL_RendererFlip flip = SDL_FLIP_NONE,
		double rotation = 0,
		Uint8 alpha = 255,
		const Camera* = nullptr,
		SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE,
		SDL_Texture* alt_texture = nullptr
	);

	void renderObjectNoMagnify
	(
		const std::string& sheet,
		sdl2::SDLRect source,
		sdl2::SDLRect dest
	);

	void stateChangeFade( int alpha );
	void colorCanvas( int color, int alpha = 255 );
	void colorCanvas();
	void clearScreen();
	void clearScreenTransparency();
	void screenBorders();
	void presentScreen();

	void newPalette( Palette palette );

	void clearSurfaces();
	void loadTexture( const std::string& sheet );

	SDL_Texture* createRenderBox( int width = Unit::WINDOW_WIDTH_PIXELS, int height = Unit::WINDOW_HEIGHT_PIXELS );
	SDL_Texture* createRenderBoxMagnified( int width = Unit::WINDOW_WIDTH_PIXELS, int height = Unit::WINDOW_HEIGHT_PIXELS );
	void setRenderTarget( SDL_Texture* texture );
	void releaseRenderTarget();
	void renderRenderBox( SDL_Texture* texture );
	void renderRenderBox( SDL_Texture* texture, sdl2::SDLRect src );
	const sdl2::SDLRect& getScreen();

	int getMaxMagnification();
	int getMagnification();
	bool isFullscreen();
	void setWindowed();
	void setFullscreen();
	void setMagnification( int magnification );
};
