
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef BOSK_SDL2
#define BOSK_SDL2

namespace sdl2
{
    struct SDL_Deleter {
      void operator()(SDL_Surface*  ptr) { if (ptr) SDL_FreeSurface(ptr); }
      void operator()(SDL_Texture*  ptr) { if (ptr) SDL_DestroyTexture(ptr); }
      void operator()(SDL_Renderer* ptr) { if (ptr) SDL_DestroyRenderer(ptr); }
      void operator()(SDL_Window*   ptr) { if (ptr) SDL_DestroyWindow(ptr); }
      void operator()(SDL_RWops*    ptr) { if (ptr) SDL_RWclose(ptr); }
    };

    // Unique Pointers
    using SDLSurface  = std::unique_ptr<SDL_Surface,  SDL_Deleter>;
    using SDLTexture  = std::unique_ptr<SDL_Texture,  SDL_Deleter>;
    using SDLRenderer = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
    using SDLWindow   = std::unique_ptr<SDL_Window,   SDL_Deleter>;
    using SDLRWops    = std::unique_ptr<SDL_RWops,    SDL_Deleter>;

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
