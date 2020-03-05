#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace sdl2
{
    class SDLColor : public SDL_Color
    {
        public:
            constexpr SDLColor( Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha )
            :
                SDL_Color( { red, green, blue, alpha } )
            {};
    };

    class SDLRect : public SDL_Rect
    {
        public:
            constexpr SDLRect( int arg_x = 0, int arg_y = 0, int arg_w = 0, int arg_h = 0 )
            :
                SDL_Rect( { arg_x, arg_y, arg_w, arg_h } )
            {};

            constexpr SDLRect( const SDLRect& c )
            :
                SDL_Rect( { c.x, c.y, c.w, c.h } )
            {};

            SDLRect& operator=( const SDLRect& c )
            {
                x = c.x;
                y = c.y;
                w = c.w;
                h = c.h;
				return *this;
            };

            constexpr SDLRect( SDLRect&& m )
            :
                SDL_Rect( { m.x, m.y, m.w, m.h } )
            {};

            SDLRect& operator=( SDLRect&& m )
            {
                x = m.x;
                y = m.y;
                w = m.w;
                h = m.h;
				return *this;
            };

            constexpr inline int halfWidth() const
            {
                return floor( w / 2 );
            };

            constexpr inline int halfHeight() const
            {
                return floor( h / 2 );
            };

            constexpr inline int centerWidth() const
            {
                return x + halfWidth();
            };

            constexpr inline int centerHeight() const
            {
                return y + halfHeight();
            };

            constexpr inline int left() const
            {
                return x;
            };

            constexpr inline int right() const
            {
                return x + w;
            };

            constexpr inline int top() const
            {
                return y;
            };

            constexpr inline int bottom() const
            {
                return y + h;
            };

            constexpr inline bool testSimpleCollision( const sdl2::SDLRect other ) const
            {
                return other.right() > x && other.x < right() && other.bottom() > y && other.y < bottom();
            };

            constexpr inline bool isLeftOf( const sdl2::SDLRect other ) const
            {
                return right() < other.x;
            };

            constexpr inline bool isRightOf( const sdl2::SDLRect other ) const
            {
                return x > other.right();
            };

            constexpr inline bool isAbove( const sdl2::SDLRect other ) const
            {
                return bottom() < other.y;
            };

            constexpr inline bool isBelow( const sdl2::SDLRect other ) const
            {
                return y > other.bottom();
            };
    };
}
