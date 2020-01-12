#pragma once

#include "mezun_sdl2.hpp"
#include <string>

class ImageGraphics
{
    public:
        ImageGraphics( std::string image, sdl2::SDLRect src, sdl2::SDLRect dest );
        ImageGraphics( std::string image, sdl2::SDLRect rect );
        void render() const;

        std::string image_;
        sdl2::SDLRect src_;
        sdl2::SDLRect dest_;
};