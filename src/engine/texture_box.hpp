#pragma once

#include <SDL2/SDL.h>
#include "mezun_sdl2.hpp"

class TextureBox
{
    public:
        TextureBox();
        void init();
        void startDrawing();
        void endDrawing();
        void render() const;
        void destroy();

    private:
        SDL_Texture* texture_;
        sdl2::SDLRect dest_;
};