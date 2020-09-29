#pragma once

#include <algorithm>
#include "line.hpp"
#include "mezun_sdl2.hpp"
#include "point.hpp"
#include <cmath>
#include <SDL2/SDL.h>

namespace Unit
{
	static constexpr int FPS                      = 60;
	static constexpr int PIXELS_PER_BLOCK         = 16;
	static constexpr int PIXELS_PER_MINIBLOCK     = 8;
	static constexpr int MINIBLOCKS_PER_BLOCK     = 2;
	static constexpr int BLOCKS_PER_SCREEN        = 25;//20;//16;
	static constexpr int MINIBLOCKS_PER_SCREEN    = BLOCKS_PER_SCREEN * 2;
	static constexpr int PIXELS_PER_SCREEN        = BLOCKS_PER_SCREEN * PIXELS_PER_BLOCK;
	static constexpr int SUBPIXELS_PER_PIXEL      = 1000;
	static constexpr int SUBPIXELS_PER_SCREEN     = PIXELS_PER_SCREEN * SUBPIXELS_PER_PIXEL;
	static constexpr int SUBPIXELS_PER_BLOCK      = PIXELS_PER_BLOCK  * SUBPIXELS_PER_PIXEL;

	static constexpr int WINDOW_WIDTH_BLOCKS      = BLOCKS_PER_SCREEN;
	static constexpr int WINDOW_WIDTH_MINIBLOCKS  = MINIBLOCKS_PER_SCREEN;
	static constexpr int WINDOW_WIDTH_PIXELS      = PIXELS_PER_SCREEN;
	static constexpr int WINDOW_WIDTH_SUBPIXELS   = SUBPIXELS_PER_SCREEN;
	static constexpr int WINDOW_HEIGHT_BLOCKS     = 14;//11;//12;
	static constexpr int WINDOW_HEIGHT_MINIBLOCKS = WINDOW_HEIGHT_BLOCKS * 2;
	static constexpr int WINDOW_HEIGHT_PIXELS     = WINDOW_HEIGHT_BLOCKS * PIXELS_PER_BLOCK;
	static constexpr int WINDOW_HEIGHT_SUBPIXELS  = WINDOW_HEIGHT_PIXELS * SUBPIXELS_PER_PIXEL;

	static constexpr int DEFAULT_ANIMATION_SPEED = 8;

	static constexpr int DEFAULT_GEM_SCORE  = -1;
	static constexpr int DEFAULT_TIME_SCORE = -1;
	static constexpr int TOTAL_FUNDS_MAX    = 999999999;
	static constexpr int TOTAL_FUNDS_MIN    = -99999999;
	static constexpr int MAX_HEART_UPGRADES = 3;

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

	inline constexpr Point PixelsToSubPixels( Point p )
	{
		return
		{
			PixelsToSubPixels( p.x ),
			PixelsToSubPixels( p.y )
		};
	};

	inline constexpr Point SubPixelsToPixels( Point p )
	{
		return
		{
			SubPixelsToPixels( p.x ),
			SubPixelsToPixels( p.y )
		};
	};

	inline constexpr Line PixelsToSubPixels( Line l )
	{
		return
		{
			PixelsToSubPixels( l.p1 ),
			PixelsToSubPixels( l.p2 )
		};
	};

	inline constexpr Line SubPixelsToPixels( Line l )
	{
		return
		{
			SubPixelsToPixels( l.p1 ),
			SubPixelsToPixels( l.p2 )
		};
	};

	enum class Layer
	{
		BEFORE_BG_1 = 0,
		BG_1,
		BEFORE_BG_2,
		BG_2,
		BEFORE_BLOCKS_1,
		BLOCKS_1,
		BEFORE_SPRITES_1,
		SPRITES_1,
		BEFORE_BLOCKS_2,
		BLOCKS_2,
		BEFORE_SPRITES_2,
		SPRITES_2,
		BEFORE_FG_1,
		FG_1,
		BEFORE_FG_2,
		FG_2,
		AFTER_FG_2,
		SUPER
	};
	static constexpr int NUMBER_OF_LAYERS = ( int )( Layer::SUPER ) + 1;

	inline constexpr Layer IntToLayer( int value )
	{
		return ( Layer )( std::max( 0, std::min( NUMBER_OF_LAYERS, value ) ) );
	};
};
