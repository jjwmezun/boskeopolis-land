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
		friend class TextComponentMarquee;

		static constexpr unsigned int DEFAULT_LINE_LENGTH = Unit::WINDOW_WIDTH_MINIBLOCKS;

		enum class FontShade
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
			FontShade color = FontShade::BLACK,
			FontAlign align = FontAlign::LEFT,
			bool center_y = false,
			unsigned int line_limit = DEFAULT_LINE_LENGTH,
			std::unique_ptr<TextComponent> component = nullptr
		);
		~Text() noexcept;
		Text( Text&& m ) noexcept;
		Text& operator=( Text&& m ) = delete;
		Text( const Text& ) = delete;
		Text& operator= ( const Text& ) = delete;

		void update();

		void render
		(
			Camera* camera = nullptr,
			FontShade color = FontShade::__NULL
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
			FontShade color = FontShade::BLACK,
			Camera* camera = nullptr
		);

		static void renderText
		(
			const std::string& text,
			int x,
			int y,
			Camera* camera = nullptr,
			FontShade color = FontShade::BLACK,
			unsigned int line_limit = DEFAULT_LINE_LENGTH,
			FontAlign align = FontAlign::LEFT,
			FontShade shadow = FontShade::__NULL,
			int char_size = 1
		);

		static std::string stringifyNum( int n );
		static std::string formatNumCommas( int n );
		static std::string formatNumDigitPadding( int n, int digits );
		static std::string formatNumCommas( std::string text );
		static std::string timeToString( int seconds, int minutes, int minutes_padding = 1 );
		static int getDigit( int n, int d, int remain = 10 );
		static int numODigits( int n );
		static int centerX( unsigned int line_length );
		static int centerY( const std::string& words, unsigned int line_limit );


	protected:
		std::string words_;
		int x_ = 0;
		int y_ = 0;


	private:
		static constexpr int CHAR_SIZE_PIXELS           = 8;
		static constexpr int NUM_OF_FONT_SHADES         = 6;
		static constexpr int CHARSET_WIDTH_MINI_BLOCKS  = 32;
		static constexpr int CHARSET_HEIGHT_MINI_BLOCKS = 32;
		static std::map<char, int> char_conversion_;

		FontShade color_;
		FontAlign align_;
		unsigned int line_limit_;
		std::unique_ptr<TextComponent> component_;

		static int frameX( unsigned int n );
		static int frameY( unsigned int n, FontShade color );
		static unsigned int colorOffset( FontShade color );
		static unsigned int testLineLength( const std::string& text, unsigned int line_length, unsigned int letters_so_far );

		unsigned int colorNum() const;
};