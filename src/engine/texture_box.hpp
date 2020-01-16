#pragma once

#include <SDL2/SDL.h>
#include "mezun_sdl2.hpp"
#include "unit.hpp"

class TextureBox
{
    public:
        TextureBox( int width = Unit::WINDOW_WIDTH_PIXELS, int height = Unit::WINDOW_HEIGHT_PIXELS, int x = 0, int y = 0 );
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
        void moveLeft( int amount = 1 );
        void moveRight( int amount = 1 );
        void moveUp( int amount = 1 );
        void moveDown( int amount = 1 );
        void setX( int value );
        void setY( int value );
        int getX() const;
        int getY() const;

    private:
        SDL_Texture* texture_;
        sdl2::SDLRect dest_;
        sdl2::SDLRect src_;
        bool token_;
};