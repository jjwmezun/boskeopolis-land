#pragma once

#include <SDL2/SDL.h>
#include "mezun_sdl2.hpp"
#include "unit.hpp"

class TextureBox final
{
    public:
        TextureBox( int width = Unit::WINDOW_WIDTH_PIXELS, int height = Unit::WINDOW_HEIGHT_PIXELS, int x = 0, int y = 0 );
        ~TextureBox();
        TextureBox( TextureBox&& t );
        TextureBox& operator=( TextureBox&& t );
        TextureBox( const TextureBox& ) = delete;
        TextureBox& operator=( const TextureBox& ) = delete;
        void init();
        void changeSize( int width, int height );
        void startDrawing() const;
        void endDrawing() const;
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
        void setHeight( int value );
        int getHeight() const;
        void setWidth( int value );
        int getWidth() const;
        void setAlpha( Uint8 alpha );
        const sdl2::SDLRect& getDest() const;
        const sdl2::SDLRect& getSrc() const;
        SDL_Texture* getTexture() const;

    private:
        SDL_Texture* texture_;
        sdl2::SDLRect dest_;
        sdl2::SDLRect src_;
        bool token_;
};