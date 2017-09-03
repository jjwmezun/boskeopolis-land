
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef BOSK_SDL2
#define BOSK_SDL2

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

            SDLRect( const SDLRect& c )
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

            SDLRect( SDLRect&& m )
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

#endif // BOSK_SDL2
