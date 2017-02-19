
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

    #include "bosk_sdl2.h"
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
            static const int FPS = 30;

            enum class SpriteSheet
            {
                CHARSET,
                BOSKO_LOGO,
                LVTILESET_UNIVERSAL,
                LVTILESET_CITY,
                LVSPRITES_AUTUMN,
                LVSPRITES_AUTUMN_CART,
                LVSPRITES_HYDRANT,
                LVBG_CITY,
                LVBG_CITY_1,
                LVBG_CITY_2,
                LVBG_CITY_3,
                LVBG_SEWER_1,
                LVBG_SEWER_2,
                LVBG_SHOP,
                LVBG_WOODS,
                LVBG_WOODS2,
                LVBG_FIRS,
                LVBG_MOUNTAIN,
                LVBG_MOUNTAIN_2,
                LVBG_MOUNTAIN_3,
                LVBG_SNOWFIRS,
                LVBG_SNOWFIRS2,
                LVBG_GRADIENT,
                LVBG_SMOG,
                LVBG_RAIN,
                LVBG_RAIN_2,
                LVBG_DARKROOM,
                LVBG_MOON,
                LVBG_CLOUDS,
                LVBG_CLOUDS_2,
                LVBG_MINTLINES,
                LVBG_CONSTELLATION,
                LVSPRITES_SAW,
                LVSPRITES_EYEGUARD,
                LVSPRITES_PLATFORM,
                LVSPRITES_BADAPPLE,
                LVSPRITES_SPIKYFRUIT,
                LVSPRITES_RACER,
                LVSPRITES_PUFFERBEE,
                LVSPRITES_ROPE,
                LVSPRITES_HANDGUN,
                LVSPRITES_BUZZSAW,
                LVSPRITES_SEWER_MONSTER,
                LVSPRITES_ICECUBE,
                LVSPRITES_ICEBLOCK,
                LVSPRITES_PENGUIN,
                LVSPRITES_FISHSTICK,
                LVSPRITES_SILLYFISH,
                LVSPRITES_CLOUD_PLATFORM,
                LVSPRITES_EGGNON,
                LVSPRITES_EGG,
                LVSPRITES_EYEBALL,
                LIMIT
            };

            Graphics( Game& game, std::vector<std::string>& args );
            ~Graphics();
            Graphics( const Graphics& )  = delete;
            Graphics& operator= ( const Graphics& ) = delete;

            const std::string imgAddress( const std::string& relative_path ) const;
            void loadTexture( SpriteSheet sheet );

            void colorCanvas();
            void clearScreen();
            void screenBorders();
            void presentScreen();
            void clearTextures();
            void clearSurfaces();
            void renderObject( SpriteSheet sheet, sdl2::SDLRect source, sdl2::SDLRect dest, SDL_RendererFlip flip = SDL_FLIP_NONE, double rotation = 0 );
            void renderRect( sdl2::SDLRect box, int color = 1, int alpha = 255 );

            void newPalette( Palette::PaletteSet palette );
            void reloadTextures();
            static void update();
            static bool nextFrame();
            int fpsMilliseconds() const;

        private:
            static Uint32 WINDOW_TYPE;
            static int FORCE_MAGNIFICATION;

            const char* graphic_addresses_[ (int)SpriteSheet::LIMIT ] =
            {
                "charset.png",
                "bosko_logo.png",
                "tileset_level_universal.png",
                "tileset_level_city.png",
                "sprites_level_autumn.png",
                "sprites_level_autumn_cart.png",
                "sprites_level_hydrant.png",
                "bg_level_city.png",
                "bg_level_city_1.png",
                "bg_level_city_2.png",
                "bg_level_city_3.png",
                "bg_level_sewer_1.png",
                "bg_level_sewer_2.png",
                "bg_level_shop.png",
                "bg_level_woods.png",
                "bg_level_woods2.png",
                "bg_level_firs.png",
                "bg_level_mountain.png",
                "bg_level_mountain_2.png",
                "bg_level_mountain_3.png",
                "bg_level_snowfirs2.png",
                "bg_level_snowfirs3.png",
                "bg_level_gradient.png",
                "bg_level_smog.png",
                "bg_level_rain.png",
                "bg_level_rain2.png",
                "bg_level_darkroom.png",
                "bg_level_moon.png",
                "bg_level_clouds.png",
                "bg_level_clouds_2.png",
                "bg_level_mintlines.png",
                "bg_level_constellation.png",
                "sprites_level_saw.png",
                "sprites_level_eyeguard.png",
                "sprites_level_platform.png",
                "sprites_level_badapple.png",
                "sprites_level_spikyfruit.png",
                "sprites_level_racer.png",
                "sprites_level_pufferbee.png",
                "sprites_level_rope.png",
                "sprites_level_handgun.png",
                "sprites_level_buzzsaw.png",
                "sprites_level_sewer_monster.png",
                "sprites_level_icecube.png",
                "sprites_level_iceblock.png",
                "sprites_level_penguin.png",
                "sprites_level_fishstick.png",
                "sprites_level_sillyfish.png",
                "sprites_level_cloud_platform.png",
                "sprites_level_eggnon.png",
                "sprites_level_egg.png",
                "sprites_level_eyeball.png"
            };

            const std::string IMG_RELATIVE_DIR = "img";
            const std::string img_path_;
            std::map<SpriteSheet, sdl2::SDLTexture> textures_;
            std::map<SpriteSheet, sdl2::SDLSurface> surfaces_;
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

