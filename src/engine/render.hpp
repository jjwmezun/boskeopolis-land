#pragma once

class Camera;
class Line;
class Palette;

#include "mezun_sdl2.hpp"
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include "unit.hpp"

namespace Render
{
	static constexpr sdl2::SDLRect window_box_ = { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS };
	void init( bool windowed, int magnification );
	void quit();

	void renderRect( const sdl2::SDLRect& box, int color = 1, int alpha = 255 );
	void renderLine( Line line, int color = 1, int alpha = 255 );
	void renderLine( int x1, int y1, int x2, int y2, int color = 1, int alpha = 255 );
	void renderLineDebug( Line line, SDL_Color color );
	void renderRectDebug( const sdl2::SDLRect& box, SDL_Color color );
	void renderRectCamera( sdl2::SDLRect box, const Camera& camera, int color, int alpha );
	void renderPointDebug( const Point& point, SDL_Color color );
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
		SDL_Texture* alt_texture = nullptr,
		const Point* rotation_center = nullptr
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
		SDL_Texture* alt_texture = nullptr,
		const Point* rotation_center = nullptr
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
		SDL_Texture* alt_texture = nullptr,
		const Point* rotation_center = nullptr
	);

	void stateChangeFade( int alpha );
	void colorCanvas( int color, int alpha = 255 );
	void colorCanvas();
	void colorCanvasForceColor( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha );
	void colorCanvasMultiply( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha );
	void colorCanvasBlend( Uint8 r, Uint8 g, Uint8 b, Uint8 alpha, SDL_BlendMode blend_mode );
	void clearScreen();
	void clearScreenTransparency();
	void tintScreen( const SDL_Color& color );
	void setTargetToCurrent();
	void renderTarget();
	void presentScreen();

	void newPalette( Palette palette );

	void clearSurfaces();
	void loadTexture( const std::string& sheet );

	SDL_Texture* createRenderBox( int width = Unit::WINDOW_WIDTH_PIXELS, int height = Unit::WINDOW_HEIGHT_PIXELS );
	void setRenderTarget( SDL_Texture* texture );
	void releaseRenderTarget();
	void renderRenderBox( SDL_Texture* texture );
	void renderRenderBox( SDL_Texture* texture, sdl2::SDLRect src );
	void renderRenderBox( SDL_Texture* texture, sdl2::SDLRect src, sdl2::SDLRect dest );
	void clearRenderBox( SDL_Texture* texture );
	const sdl2::SDLRect& getScreen();
	SDL_Surface* getSurface( std::string sheet );

	int getMaxMagnification();
	int getMagnification();
	bool isFullscreen();
	void setWindowed();
	void setFullscreen();
	void setMagnification( int magnification );

	void turnOnTransitionPalette( Palette pal1, Palette pal2 );
	void setPaletteTransAlpha( Uint8 alpha );
	bool hasTransPalette();
	void swapTransPalette();
};
