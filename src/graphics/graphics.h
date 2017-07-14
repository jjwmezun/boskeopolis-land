#pragma once

class Game;

#include "mezun_sdl2.h"
#include <iostream>
#include "palette.h"
#include <SDL2/SDL.h>
#include "timers/timer_repeat.h"
#include <map>
#include <vector>

class Graphics
{
	public:
		Graphics( Game& game, const std::vector<std::string>& args );
		Graphics( const Graphics& ) = delete;
		Graphics& operator= ( const Graphics& ) = delete;
		Graphics( Graphics&& ) = delete;
		Graphics&& operator= ( Graphics&& ) = delete;

		void colorCanvas();
		void clearScreen();
		void screenBorders();
		void presentScreen();
		void quit();

		void renderObject
		(
			const std::string& sheet,
			sdl2::SDLRect source,
			sdl2::SDLRect dest,
			SDL_RendererFlip flip = SDL_FLIP_NONE,
			double rotation = 0
		);
		void renderRect( const sdl2::SDLRect& box, int color = 1, int alpha = 255 );

		void newPalette( Palette palette );
		void clearSurfaces();


	private:
		static constexpr Uint8 FULL_OPACITY = 255;

		static Uint32 WINDOW_TYPE;
		static int FORCE_MAGNIFICATION;

		const std::string IMG_RELATIVE_DIR = "img";

		const std::string img_path_;
		std::map<std::string, SDL_Texture*> textures_;
		std::map<std::string, SDL_Surface*> surfaces_;
		SDL_Renderer* renderer_ = nullptr;
		SDL_Window* window_ = nullptr;
		sdl2::SDLRect screen_;
		static TimerRepeat animation_frame_;
		std::unique_ptr<const Palette> palette_;

		void consoleArguments( const std::vector<std::string>& args );
		void createRenderer();
		void createWindow();

		int magnification_ = 1;
		int left_edge_ = 0;
		int top_edge_ = 0;
		sdl2::SDLRect top_bar_ = { 0, 0, 0, 0 };
		sdl2::SDLRect bottom_bar_ = { 0, 0, 0, 0 };
		sdl2::SDLRect left_bar_ = { 0, 0, 0, 0 };
		sdl2::SDLRect right_bar_ = { 0, 0, 0, 0 };

		int magnified( int n ) const;
		void adjustMagnification();
		int calculateMagnification( int monitor_width, int monitor_height );
		void adjustScreen( int monitor_width, int monitor_height );
		void calculateScreenEdges( int monitor_width, int monitor_height );
		void adjustBorderBars( int monitor_width, int monitor_height );

		void loadSurface( const std::string& sheet );
		void loadTexture( const std::string& sheet );
		void clearTextures();
		void reloadTextures();

		const std::string imgAddress( const std::string& relative_path ) const;
		const std::string setImgPath( Game& game );
		sdl2::SDLRect sourceRelativeToScreen( const sdl2::SDLRect& source ) const;
};