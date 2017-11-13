#pragma once

class Camera;
class TextComponent;

#include "counter.hpp"
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include "text_component_marquee.hpp"
#include "timers/timer_repeat.hpp"
#include "unit.hpp"

class Text
{
	public:

	static constexpr unsigned int DEFAULT_LINE_LENGTH = Unit::WINDOW_WIDTH_MINIBLOCKS;

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

	Text
	(
		std::string words,
		int x = 0,
		int y = 0,
		FontColor color = FontColor::BLACK,
		FontAlign align = FontAlign::LEFT,
		bool center_y = false,
		unsigned int line_limit = DEFAULT_LINE_LENGTH,
		std::unique_ptr<TextComponent> component = nullptr,
		FontColor shadow = FontColor::__NULL
	);
	~Text() noexcept;
	Text( Text&& m ) noexcept;
	Text& operator=( Text&& m ) = delete;
	Text( const Text& ) = delete;
	Text& operator= ( const Text& ) = delete;

	void update();
	void render
	(
		const Camera* camera = nullptr,
		FontColor color = FontColor::__NULL
	) const;

	int right() const;
	unsigned int width() const;
	int x() const;

	static void renderNumber
	(
		int n,
		int x,
		int y,
		int digits = -1,
		FontColor color = FontColor::BLACK,
		const Camera* camera = nullptr
	);

	static void renderText
	(
		const std::string& text,
		int x,
		int y,
		const Camera* camera = nullptr,
		FontColor color = FontColor::BLACK,
		unsigned int line_limit = DEFAULT_LINE_LENGTH,
		FontAlign align = FontAlign::LEFT,
		FontColor shadow = FontColor::__NULL,
		int char_size = 1
	);
	static void renderChar( char letter, FontColor color, const sdl2::SDLRect& dest );

	static std::string stringifyNum( int n );
	static std::string formatNumCommas( int n );
	static std::string formatNumDigitPadding( int n, int digits );
	static std::string formatNumCommas( std::string text );
	static std::string timeToString( int seconds, int minutes, int minutes_padding = 1 );
	static int numODigits( int n );
	static int centerX( unsigned int line_length );
	static int centerY( const std::string& words, unsigned int line_limit );

	// Get d digit place for n #.
	static constexpr int getDigit( int n, int d, int remain )
	{
		return int( floor( n / pow( 10, d - 1 ) ) ) % remain;
	};

	std::string words_;
	std::unique_ptr<TextComponent> component_;
	FontColor color_;
	FontColor shadow_;
	FontAlign align_;
	unsigned int line_limit_;
	int x_ = 0;
	int y_ = 0;
};