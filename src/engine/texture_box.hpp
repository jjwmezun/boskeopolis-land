#pragma once

#include <SDL2/SDL.h>
#include "mezun_sdl2.hpp"

class TextureBox
{
    public:
        TextureBox();
        ~TextureBox();
        TextureBox( TextureBox&& t ) = delete;
        TextureBox& operator=( TextureBox&& t );
        TextureBox( const TextureBox& ) = delete;
        TextureBox& operator=( const TextureBox& ) = delete;
        void init();
        void startDrawing();
        void endDrawing();
        void render() const;
        void destroy();

    private:
        SDL_Texture* texture_;
        sdl2::SDLRect dest_;
        bool token_;
};