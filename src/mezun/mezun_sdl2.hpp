#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace sdl
{
    class color : public SDL_Color
    {
        public:
            constexpr color( Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255 )
            :
                SDL_Color( { red, green, blue, alpha } )
            {};
    };

    class rect : public SDL_Rect
    {
        public:
            constexpr rect( int arg_x = 0, int arg_y = 0, int arg_w = 0, int arg_h = 0 )
            :
                SDL_Rect( { arg_x, arg_y, arg_w, arg_h } )
            {};

            constexpr rect( const rect& c )
            :
                SDL_Rect( { c.x, c.y, c.w, c.h } )
            {};

            rect& operator=( const rect& c );
            rect( rect&& m );
            rect& operator=( rect&& m );

            constexpr int halfWidth() const
            {
                return floor( w / 2 );
            };

            constexpr int halfHeight() const
            {
                return floor( h / 2 );
            };

            constexpr int centerWidth() const
            {
                return x + halfWidth();
            };

            constexpr int centerHeight() const
            {
                return y + halfHeight();
            };

            constexpr int left() const
            {
                return x;
            };

            constexpr int right() const
            {
                return x + w;
            };

            constexpr int top() const
            {
                return y;
            };

            constexpr int bottom() const
            {
                return y + h;
            };
    };
}

std::ostream& operator<<( std::ostream& os, const sdl::color& color );