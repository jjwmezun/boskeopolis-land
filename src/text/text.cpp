#include <iostream>
#include <algorithm>
#include "camera.hpp"
#include <map>
#include "render.hpp"
#include <sstream>
#include <stdexcept>
#include "text.hpp"

namespace Text
{
	static constexpr int NUM_OF_FONT_SHADES         = 6;
	static constexpr int CHARSET_WIDTH_MINI_BLOCKS  = 32;
	static constexpr int CHARSET_HEIGHT_MINI_BLOCKS = 32;
	static std::map<char, int> char_conversion_ =
	{
		{ '0', 0 },
		{ '1', 1 },
		{ '2', 2 },
		{ '3', 3 },
		{ '4', 4 },
		{ '5', 5 },
		{ '6', 6 },
		{ '7', 7 },
		{ '8', 8 },
		{ '9', 9 },
		{ 'a', 10 },
		{ 'A', 10 },
		{ 'b', 11 },
		{ 'B', 11 },
		{ 'c', 12 },
		{ 'C', 12 },
		{ 'd', 13 },
		{ 'D', 13 },
		{ 'e', 14 },
		{ 'E', 14 },
		{ 'f', 15 },
		{ 'F', 15 },
		{ 'g', 16 },
		{ 'G', 16 },
		{ 'h', 17 },
		{ 'H', 17 },
		{ 'i', 18 },
		{ 'I', 18 },
		{ 'j', 19 },
		{ 'J', 19 },
		{ 'k', 20 },
		{ 'K', 20 },
		{ 'l', 21 },
		{ 'L', 21 },
		{ 'm', 22 },
		{ 'M', 22 },
		{ 'n', 23 },
		{ 'N', 23 },
		{ 'o', 24 },
		{ 'O', 24 },
		{ 'p', 25 },
		{ 'P', 25 },
		{ 'q', 26 },
		{ 'Q', 26 },
		{ 'r', 27 },
		{ 'R', 27 },
		{ 's', 28 },
		{ 'S', 28 },
		{ 't', 29 },
		{ 'T', 29 },
		{ 'u', 30 },
		{ 'U', 30 },
		{ 'v', 31 },
		{ 'V', 31 },
		{ 'w', 32 },
		{ 'W', 32 },
		{ 'x', 33 },
		{ 'X', 33 },
		{ 'y', 34 },
		{ 'Y', 34 },
		{ 'z', 35 },
		{ 'Z', 35 },
		{ '?', 36 },
		{ '!', 37 },
		{ '¡', 38 },
		{ '¿', 39 },
		{ '@', 40 },
		{ '#', 41 },
		{ '$', 42 },
		{ '%', 43 },
		{ '^', 44 },
		{ '&', 45 },
		{ '*', 46 },
		{ '(', 47 },
		{ ')', 48 },
		{ '[', 49 },
		{ ']', 50 },
		{ ',', 51 },
		{ '.', 52 },
		{ '/', 53 },
		{ '\'', 54 },
		{ '’', 54 },
		{ ':', 55 },
		{ ';', 56 },
		{ '{', 57 },
		{ '}', 58 },
		{ '-', 59 },
		{ '_', 60 },
		{ '=', 61 },
		{ '"', 62 },
		{ '/', 63 },
		{ '\\', 64 },
		{ '~', 65 },
		{ '|', 66 },
		{ '+', 67 },
		{ 'í', 74 },
		{ '\t', 68 }, // Use this for PTS char.
		{ ' ', 127 }
	};

	unsigned int testLineLength( const std::string& text, unsigned int line_length, unsigned int letters_so_far );
	static constexpr unsigned int colorOffset( FontColor color )
	{
		return ( color == FontColor::__NULL ) ? 0 : ( unsigned int )( color ) * CHARSET_HEIGHT_MINI_BLOCKS;
	};
	static constexpr int frameX( unsigned int n )
	{
		return Unit::MiniBlocksToPixels( ( int )( n ) % CHARSET_WIDTH_MINI_BLOCKS );
	};
	static constexpr int frameY( unsigned int n, FontColor color )
	{
		return colorOffset( color ) + Unit::MiniBlocksToPixels( floor( ( int )( n ) / CHARSET_WIDTH_MINI_BLOCKS ) );
	};

	void renderText
	(
		const std::string& text,
		int x,
		int y,
		const Camera* camera,
		FontColor color,
		unsigned int line_limit,
		FontAlign align,
		FontColor shadow,
		int char_size,
		int chars_to_render
	)
	{
		chars_to_render = ( chars_to_render == -1 ) ? text.length() : chars_to_render;
		renderText( text.c_str(), chars_to_render, x, y, camera, color, line_limit, align, shadow, char_size );
	};

	void renderText
	(
		const char* text,
		int chars_to_render,
		int x,
		int y,
		const Camera* camera,
		FontColor color,
		unsigned int line_limit,
		FontAlign align,
		FontColor shadow,
		int char_size
	)
	{
		if ( line_limit == 0 )
		{
			line_limit = DEFAULT_LINE_LENGTH;
		}

		unsigned int line = 0;
		unsigned int letters_so_far = 0;

		while ( letters_so_far < chars_to_render )
		{
			const unsigned int line_length = testLineLength( text, line_limit, letters_so_far );
			const unsigned int absolute_line_limit = line_length + letters_so_far;

			if ( align == FontAlign::CENTER )
			{
				x = centerX( line_length * CHAR_SIZE_PIXELS * char_size );
			}

			for ( unsigned int c = letters_so_far; c < absolute_line_limit; ++c )
			{
				if ( c > chars_to_render ) return;

				// Newline
				if ( text[ c ] == '\n' )
				{
					break;
				}
				// Character quirk: function treats characters below 0 as 2 chars with 0 as the 1st. This skips the 0.
				else if ( text[ c ] < 0 )
				{
					++c;
				}

				const char letter = text[ c ];
				const int char_x = ( CHAR_SIZE_PIXELS * char_size * ( c - ( int )( letters_so_far ) ) ) + x;
				const int char_y = ( CHAR_SIZE_PIXELS * char_size * ( int )( line ) ) + y;

				sdl2::SDLRect dest =
				{
					char_x,
					char_y,
					CHAR_SIZE_PIXELS * char_size,
					CHAR_SIZE_PIXELS * char_size
				};

				if ( camera != nullptr )
				{
					dest.x = camera->relativeX( dest );
					dest.y = camera->relativeY( dest );
				}

				if ( shadow != FontColor::__NULL )
				{
					renderChar( letter, shadow, { dest.x + char_size, dest.y + char_size, dest.w, dest.h } );
				}
				renderChar( letter, color, dest );
			}

			++line;
			letters_so_far += line_length;
		}

	};

	void renderChar( char letter, FontColor color, const sdl2::SDLRect& dest )
	{
		const int frame = char_conversion_[ letter ];
		Render::renderObject
		(
			"charset.png",
			{
				frameX( frame ),
				frameY( frame, color ),
				CHAR_SIZE_PIXELS,
				CHAR_SIZE_PIXELS
			},
			dest
		);
	};

	int centerX( unsigned int line_length )
	{
		// Center = half o' line length left o' middle (half window width), half on right.
		// [     *    |    )     ]
		// * is left-most point we want to return.
		return
			floor ( Unit::WINDOW_WIDTH_PIXELS / 2 ) -
			floor ( line_length / 2 );
	};

	int centerY( const std::string& words, unsigned int line_limit )
	{
		unsigned int text_height = 1;

		unsigned int col = 0;
		for ( unsigned int i = 0; i < words.length(); ++i )
		{
			if ( words[ i ] == '\n' )
			{
				++text_height;
				col = 0;
			}
			else
			{
				if ( col >= line_limit )
				{
					++text_height;
					col = 0;
				}
				else
				{
					++col;
				}
			}
		}

		return Unit::MiniBlocksToPixels( floor( ( Unit::WINDOW_HEIGHT_MINIBLOCKS - text_height ) / 2 ) ) + ( CHAR_SIZE_PIXELS / 2 );
	};

	void renderNumber( int n, int x, int y, int digits, FontColor color, const Camera* camera, FontColor shadow )
	{
		renderText( formatNumDigitPadding( n, digits ), x, y, camera, color, DEFAULT_LINE_LENGTH, FontAlign::LEFT, shadow );
	};

	// Determine how many digit places n # has.
	int numODigits( int n )
	{
		const int LOOP_LIMIT = 99; // Prevent endless loops.

		if ( n == 0 )
		{
			return 1;
		}
		else
		{
			for ( int i = 0; i < LOOP_LIMIT; ++i )
			{
				if ( abs( n ) < pow( 10, i ) )
				{
					return i;
				}
			}

			// Loop shouldn't reach limit.
			throw std::logic_error( "numODigits loop doesn't end." );
		}

		return -1;
	};

	std::string formatNumCommas( int n )
	{
		return formatNumCommas( stringifyNum( n ) );
	};

	// Add commas to # string 'tween every 3 digits.
	std::string formatNumCommas( std::string text )
	{
		const int NUM_O_NUMS_TWEEN_COMMAS = 3;

		int difference = text.length() - NUM_O_NUMS_TWEEN_COMMAS;

		while ( difference > 0 )
		{
			text.insert( difference, "," );
			difference -= NUM_O_NUMS_TWEEN_COMMAS;
		}

		return text;
	};

	// Pad # n to given digits so that # string is always a consistent length,
	// filling leading space with 0s.
	std::string formatNumDigitPadding( int n, int digits )
	{
		std::string text = stringifyNum( std::abs( n ) );

		if ( digits > 0 )
		{
			int difference = digits - text.size();

			// If n is negative, leave 1 char space for - sign.
			if ( n < 0 )
			{
				--difference;
			}

			if ( difference > 0 )
			{
				// For every extra digit, add a leading 0 as padding.
				for ( int i = 0; i < difference; ++i )
				{
					text.insert( 0, "0" );
				}
			}
		}

		// If n is negative, add - sign before everything.
		if ( n < 0 )
		{
			text.insert( 0, "-" );
		}

		return text;
	};

	std::string stringifyNum( int n )
	{
		std::ostringstream oss;
		oss << n;
		std::string text = oss.str();
		return text;
	};

	std::string timeToString( int seconds, int minutes, int minutes_padding )
	{
		return formatNumDigitPadding( minutes, minutes_padding ) + ":" + formatNumDigitPadding( seconds, 2 );
	};

	unsigned int testLineLength( const std::string& text, unsigned int line_length, unsigned int letters_so_far )
	{
		unsigned int limit = std::min( ( int )( line_length ), ( int )( text.length() ) - ( int )( letters_so_far ) );

		for( unsigned int line_letter = 0; line_letter < limit; ++line_letter )
		{
			const unsigned int absolute_letter = letters_so_far + line_letter;

			if ( text[ absolute_letter ] == '\n' )
			{
				// While it may seem simpler to just return the line_letter & not test for new line in rendering code,
				// this causes a ne'er-ending loop if a line starts with \n,
				// since the character is ne'er passed, & thus the code keeps testing for it fore'er.
				return line_letter + 1;
			}
			else if ( text[ absolute_letter ] < 0 )
			{
				//++limit;
				// This code causes program to crash with out_of_range exception,
				// but it may still offset text if special characters are used.
				// Further testing needed.
			}
		}

		return limit;
	};

	std::string autoformat( std::string words, unsigned int line_limit )
	{
		int n = 0;
		int x = 0;
		int line_length = ( int )( line_limit );
		int text_length = ( int )( words.length() );

		while ( n != text_length )
		{
			if ( x == line_length )
			{
				if ( words[ n ] == ' ' || words[ n ] == '\n' )
				{
					words.erase( words.begin() + n );
					++x;
				}
				else
				{
					while ( true )
					{
						if ( words[ n ] == ' ' )
						{
							words[ n ] = '\n';
							++n; // Back to 1 after end o' this line.
							x = 0; // Back to start o' new line.
							break;
						}
						// Reached start o' line without finding space.
						else if ( x == 0 )
						{
							words.insert( words.begin() + n + line_length - 2, '-' );
							n += line_length - 1;
							break;
						}
						--x;
						--n;
					}
				}
			}
			else if ( words[ n ] == '\n' )
			{
				x = 0;
				++n;
			}
			else
			{
				++x;
				++n;
			}
		}
		return words;
	};
}
