#pragma once

#include "mezun_sdl2.hpp"
#include <cmath>
#include <SDL2/SDL.h>

namespace Unit
{
	constexpr int FPS                      = 60;
	constexpr int PIXELS_PER_BLOCK         = 16;
	constexpr int PIXELS_PER_MINIBLOCK     = 8;
	constexpr int MINIBLOCKS_PER_BLOCK     = 2;
	constexpr int BLOCKS_PER_SCREEN        = 25;//20;//16;
	constexpr int MINIBLOCKS_PER_SCREEN    = BLOCKS_PER_SCREEN * 2;
	constexpr int PIXELS_PER_SCREEN        = BLOCKS_PER_SCREEN * PIXELS_PER_BLOCK;
	constexpr int SUBPIXELS_PER_PIXEL      = 1000;
	constexpr int SUBPIXELS_PER_SCREEN     = PIXELS_PER_SCREEN * SUBPIXELS_PER_PIXEL;
	constexpr int SUBPIXELS_PER_BLOCK      = PIXELS_PER_BLOCK  * SUBPIXELS_PER_PIXEL;

	constexpr int WINDOW_WIDTH_BLOCKS      = BLOCKS_PER_SCREEN;
	constexpr int WINDOW_WIDTH_MINIBLOCKS  = MINIBLOCKS_PER_SCREEN;
	constexpr int WINDOW_WIDTH_PIXELS      = PIXELS_PER_SCREEN;
	constexpr int WINDOW_WIDTH_SUBPIXELS   = SUBPIXELS_PER_SCREEN;
	constexpr int WINDOW_HEIGHT_BLOCKS     = 14;//11;//12;
	constexpr int WINDOW_HEIGHT_MINIBLOCKS = WINDOW_HEIGHT_BLOCKS * 2;
	constexpr int WINDOW_HEIGHT_PIXELS     = WINDOW_HEIGHT_BLOCKS * PIXELS_PER_BLOCK;
	constexpr int WINDOW_HEIGHT_SUBPIXELS  = WINDOW_HEIGHT_PIXELS * SUBPIXELS_PER_PIXEL;

	constexpr int DEFAULT_ANIMATION_SPEED = 8;

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
