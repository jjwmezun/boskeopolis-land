#pragma once

class Camera;

#include <SDL2/SDL.h>
#include <string>
#include "unit.hpp"

namespace Text
{
	constexpr unsigned int DEFAULT_LINE_LENGTH = Unit::WINDOW_WIDTH_MINIBLOCKS;
	constexpr int CHAR_SIZE_PIXELS = 8;
	enum class FontColor
	{
		BLACK = 0,
		DARK_GRAY = 1,
		DARK_MID_GRAY = 2,
		LIGHT_MID_GRAY = 3,
		LIGHT_GRAY = 4,
		WHITE = 5,
		__NULL = 6
	};

	enum class FontAlign
	{
		LEFT,
		CENTER,
		RIGHT
	};

	void renderNumber
	(
		int n,
		int x,
		int y,
		int digits = -1,
		FontColor color = FontColor::BLACK,
		const Camera* camera = nullptr
	);

	void renderText
	(
		const std::string& text,
		int x = 0,
		int y = 0,
		const Camera* camera = nullptr,
		FontColor color = FontColor::BLACK,
		unsigned int line_limit = DEFAULT_LINE_LENGTH,
		FontAlign align = FontAlign::LEFT,
		FontColor shadow = FontColor::__NULL,
		int char_size = 1,
		int chars_to_render = -1
	);
	void renderText
	(
		const char* text,
		int chars_to_render,
		int x = 0,
		int y = 0,
		const Camera* camera = nullptr,
		FontColor color = FontColor::BLACK,
		unsigned int line_limit = DEFAULT_LINE_LENGTH,
		FontAlign align = FontAlign::LEFT,
		FontColor shadow = FontColor::__NULL,
		int char_size = 1
	);
	void renderChar( char letter, FontColor color, const sdl2::SDLRect& dest );

	std::string stringifyNum( int n );
	std::string formatNumCommas( int n );
	std::string formatNumDigitPadding( int n, int digits );
	std::string formatNumCommas( std::string text );
	std::string timeToString( int seconds, int minutes, int minutes_padding = 1 );
	int numODigits( int n );
	int centerX( unsigned int line_length );
	int centerY( const std::string& words, unsigned int line_limit );

	// Get d digit place for n #.
	constexpr int getDigit( int n, int d, int remain )
	{
		return int( floor( n / pow( 10, d - 1 ) ) ) % remain;
	};
};