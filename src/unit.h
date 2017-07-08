
#ifndef UNIT_H
#define UNIT_H

#include "mezun_sdl2.h"
#include <cmath>
#include <SDL2/SDL.h>

// Name
//===================================
//
// Unit
//

    namespace Unit
    {
        const int FPS                      = 60;
        const int PIXELS_PER_BLOCK         = 16;
        const int PIXELS_PER_MINIBLOCK     = 8;
        const int MINIBLOCKS_PER_BLOCK     = 2;
        const int BLOCKS_PER_SCREEN        = 20;//16;
        const int MINIBLOCKS_PER_SCREEN    = BLOCKS_PER_SCREEN * 2;
        const int PIXELS_PER_SCREEN        = BLOCKS_PER_SCREEN * PIXELS_PER_BLOCK;
        const int SUBPIXELS_PER_PIXEL      = 1000;
        const int SUBPIXELS_PER_SCREEN     = PIXELS_PER_SCREEN * SUBPIXELS_PER_PIXEL;

        const int WINDOW_WIDTH_BLOCKS      = BLOCKS_PER_SCREEN;
        const int WINDOW_WIDTH_MINIBLOCKS  = MINIBLOCKS_PER_SCREEN;
        const int WINDOW_WIDTH_PIXELS      = PIXELS_PER_SCREEN;
        const int WINDOW_WIDTH_SUBPIXELS   = SUBPIXELS_PER_SCREEN;
        const int WINDOW_HEIGHT_BLOCKS     = 11;//12;
        const int WINDOW_HEIGHT_MINIBLOCKS = WINDOW_HEIGHT_BLOCKS * 2;
        const int WINDOW_HEIGHT_PIXELS     = WINDOW_HEIGHT_BLOCKS * PIXELS_PER_BLOCK;
        const int WINDOW_HEIGHT_SUBPIXELS  = WINDOW_HEIGHT_PIXELS * SUBPIXELS_PER_PIXEL;

        inline constexpr int PixelsToBlocks( int p )
        {
            return round( p / PIXELS_PER_BLOCK );
        };

        inline constexpr int BlocksToPixels( int b )
        {
            return b * PIXELS_PER_BLOCK;
        };

        inline constexpr int PixelsToMiniBlocks( int p )
        {
            return round( p / PIXELS_PER_MINIBLOCK );
        };

        inline constexpr int MiniBlocksToPixels( int mb )
        {
            return mb * PIXELS_PER_MINIBLOCK;
        };

        inline constexpr int MiniBlocksToBlocks( int mb )
        {
            return round( mb / MINIBLOCKS_PER_BLOCK );
        };

        inline constexpr int BlocksToMiniBlocks( int b )
        {
            return b * MINIBLOCKS_PER_BLOCK;
        };

        inline constexpr int ScreensToBlocks( int s )
        {
            return s * BLOCKS_PER_SCREEN;
        };

        inline constexpr int BlocksToScreens( int b )
        {
            return round( b / BLOCKS_PER_SCREEN );
        };

        inline constexpr int ScreensToMiniBlocks( int s )
        {
            return s * MINIBLOCKS_PER_SCREEN;
        };

        inline constexpr int MiniBlocksToScreens( int mb )
        {
            return round( mb / MINIBLOCKS_PER_SCREEN );
        };

        inline constexpr int ScreensToPixels( int s )
        {
            return s * PIXELS_PER_SCREEN;
        };

        inline constexpr int PixelsToScreens( int p )
        {
            return round( p / PIXELS_PER_SCREEN );
        };

        inline constexpr int SubPixelsToPixels( int sp )
        {
            return round( sp / SUBPIXELS_PER_PIXEL );
        };

        inline constexpr int PixelsToSubPixels( int p )
        {
            return p * SUBPIXELS_PER_PIXEL;
        };

        inline constexpr int BlocksToSubPixels( int b )
        {
            return PixelsToSubPixels( BlocksToPixels( b ) );
        };

        inline constexpr int SubPixelsToBlocks( int sp )
        {
            return PixelsToBlocks( SubPixelsToPixels( sp ) );
        };

        inline constexpr sdl2::SDLRect PixelsToSubPixels( const sdl2::SDLRect& r )
        {
            return
            {
                PixelsToSubPixels( r.x ),
                PixelsToSubPixels( r.y ),
                PixelsToSubPixels( r.w ),
                PixelsToSubPixels( r.h )
            };
        };

        inline constexpr sdl2::SDLRect SubPixelsToPixels( const sdl2::SDLRect& r )
        {
            return
            {
                SubPixelsToPixels( r.x ),
                SubPixelsToPixels( r.y ),
                SubPixelsToPixels( r.w ),
                SubPixelsToPixels( r.h )
            };
        };
    };

#endif // UNIT_H
