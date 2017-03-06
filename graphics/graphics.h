
// Name
//===================================
//
// Graphics
//

#ifndef GRAPHICS_H
#define GRAPHICS_H


// FORWARD DECLARATIONS
//===================================

    class Game;


// DEPENDENCIES
//===================================

    #include "mezun_sdl2.h"
    #include <iostream>
    #include "palette.h"
    #include <SDL2/SDL.h>
    #include "timers/timer_repeat.h"
    #include <map>
	#include <vector>


// CLASS
//===================================

    class Graphics
    {
        public:
            Graphics( Game& game, std::vector<std::string>& args );
            ~Graphics();
            Graphics( const Graphics& ) = delete;
            Graphics& operator= ( const Graphics& ) = delete;
            Graphics( Graphics&& ) = delete;
            Graphics&& operator= ( Graphics&& ) = delete;

            const std::string imgAddress( const std::string& relative_path ) const;
            void loadTexture( const std::string& sheet );

            void colorCanvas();
            void clearScreen();
            void screenBorders();
            void presentScreen();
            void clearTextures();
            void clearSurfaces();
            void renderObject
			(
				const std::string& sheet,
				sdl2::SDLRect source,
				sdl2::SDLRect dest,
				SDL_RendererFlip flip = SDL_FLIP_NONE,
				double rotation = 0
			);
			
            void renderRect( sdl2::SDLRect box, int color = 1, int alpha = 255 );

            void newPalette( Palette palette );
            void reloadTextures();
            static void update();
            static bool nextFrame();
			

        private:
            static Uint32 WINDOW_TYPE;
            static int FORCE_MAGNIFICATION;

            const std::string IMG_RELATIVE_DIR = "img";
			
            const std::string img_path_;
            std::map<std::string, sdl2::SDLTexture> textures_;
            std::map<std::string, sdl2::SDLSurface> surfaces_;
            sdl2::SDLRenderer renderer_ = nullptr;
            sdl2::SDLWindow   window_ = nullptr;
            sdl2::SDLRect screen_;
            sdl2::SDLTexture screen_surface_;
            static TimerRepeat animation_frame_;
            std::unique_ptr<const Palette> palette_;

            int magnification_ = 1;
            int left_edge_ = 0;
            int top_edge_ = 0;

            sdl2::SDLRect top_bar_ = { 0, 0, 0, 0 };
            sdl2::SDLRect bottom_bar_ = { 0, 0, 0, 0 };
            sdl2::SDLRect left_bar_ = { 0, 0, 0, 0 };
            sdl2::SDLRect right_bar_ = { 0, 0, 0, 0 };

            int magnified( int n ) const;
            void adjustMagnification();
            void adjustScreen();

            bool createRenderer();
            bool createWindow();
            const std::string setImgPath( Game& game );

            sdl2::SDLRect sourceRelativeToScreen( sdl2::SDLRect source ) const;
    };

#endif // GRAPHICS_H

