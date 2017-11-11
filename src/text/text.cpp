#include <iostream>
#include <algorithm>
#include "camera.hpp"
#include "render.hpp"
#include <sstream>
#include <stdexcept>
#include "text.hpp"

std::map<char, int> Text::char_conversion_ =
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


// METHODS
//===================================

Text::Text
(
	std::string words,
	int x,
	int y,
	FontShade color,
	FontAlign align,
	bool center_y,
	unsigned int line_limit,
	std::unique_ptr<TextComponent> component
)
:
	words_ ( words ),
	x_ ( x ),
	y_ ( ( center_y ) ? centerY( words_, ( ( line_limit == 0 ) ? DEFAULT_LINE_LENGTH : line_limit ) ) : y ),
	color_ ( color ),
	align_ ( align ),
	component_ ( std::move( component ) ),
	line_limit_ ( ( line_limit == 0 ) ? DEFAULT_LINE_LENGTH : line_limit )
{};

Text::~Text() noexcept {};

Text::Text( Text&& m ) noexcept
:
	words_ ( m.words_ ),
	x_ ( m.x_ ),
	y_ ( m.y_ ),
	color_ ( m.color_ ),
	align_ ( m.align_ ),
	component_ ( std::move( m.component_ ) ),
	line_limit_ ( m.line_limit_ )
{};

void Text::renderText
(
	const std::string& text,
	int x,
	int y,
	Camera* camera,
	FontShade color,
	unsigned int line_limit,
	FontAlign align,
	FontShade shadow,
	int char_size
)
{
	if ( line_limit == 0 )
	{
		line_limit = DEFAULT_LINE_LENGTH;
	}

	unsigned int line = 0;
	unsigned int letters_so_far = 0;

	while ( letters_so_far < text.length() )
	{
		const unsigned int line_length = testLineLength( text, line_limit, letters_so_far );
		const unsigned int absolute_line_limit = line_length + letters_so_far;

		if ( align == FontAlign::CENTER )
		{
			x = centerX( line_length * CHAR_SIZE_PIXELS * char_size );
		}

		for ( unsigned int c = letters_so_far; c < absolute_line_limit; ++c )
		{
			// Newline
			if ( text.at( c ) == '\n' )
			{
				break;
			}
			// Character quirk: function treats characters below 0 as 2 chars with 0 as the 1st. This skips the 0.
			else if ( text.at( c ) < 0 )
			{
				++c;
			}

			const int frame = char_conversion_[ text[ c ] ];
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
		}

		++line;
		letters_so_far += line_length;
	}

};

void Text::render( Camera* camera, FontShade color ) const
{
	if ( color == FontShade::__NULL )
	{
		color = color_;
	}

	renderText( words_, x_, y_, camera, color, line_limit_, align_ );
};

int Text::centerX( unsigned int line_length )
{	
	// Center = half o' line length left o' middle (half window width), half on right.
	// [     *    |    )     ]
	// * is left-most point we want to return.
	return
		floor ( Unit::WINDOW_WIDTH_PIXELS / 2 ) -
		floor ( line_length / 2 );
};

int Text::centerY( const std::string& words, unsigned int line_limit )
{
	unsigned int text_height = 0;

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

	return Unit::MiniBlocksToPixels
	(
		ceil( ( Unit::WINDOW_HEIGHT_MINIBLOCKS - text_height ) / 2 )
	);
};

unsigned int Text::colorOffset( FontShade color )
{
	if ( color == FontShade::__NULL )
	{
		return 0;
	}
	else
	{
		return ( unsigned int )( color ) * CHARSET_HEIGHT_MINI_BLOCKS;
	}
};

unsigned int Text::colorNum() const
{
	if ( color_ == FontShade::__NULL )
	{
		return 0;
	}
	else
	{
		return ( unsigned int )( color_ );
	}
};

int Text::frameX( unsigned int n )
{
	return Unit::MiniBlocksToPixels( ( int )( n ) % CHARSET_WIDTH_MINI_BLOCKS );
};

int Text::frameY( unsigned int n, FontShade color )
{
	return colorOffset( color ) + Unit::MiniBlocksToPixels( floor( ( int )( n ) / CHARSET_WIDTH_MINI_BLOCKS ) );
};

void Text::renderNumber( int n, int x, int y, int digits, FontShade color, Camera* camera )
{
	renderText( formatNumDigitPadding( n, digits ), x, y, camera, color );
};

// Get d digit place for n #.
int Text::getDigit( int n, int d, int remain )
{
	return int( floor( n / pow( 10, d - 1 ) ) ) % remain;
};

// Determine how many digit places n # has.
int Text::numODigits( int n )
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
		throw std::logic_error( "Text::numODigits loop doesn't end." );
	}

	return -1;
};

void Text::update()
{
	if ( component_ )
	{
		component_->update( *this );
	}
};

std::string Text::formatNumCommas( int n )
{
	return formatNumCommas( stringifyNum( n ) );
};

// Add commas to # string 'tween every 3 digits.
std::string Text::formatNumCommas( std::string text )
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
std::string Text::formatNumDigitPadding( int n, int digits )
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

std::string Text::stringifyNum( int n )
{
	std::ostringstream oss;
	oss << n;
	std::string text = oss.str();
	return text;
};

int Text::right() const
{
	return x_ + width();
};

unsigned int Text::width() const
{
	return words_.size() * ( unsigned int )( CHAR_SIZE_PIXELS );
};

int Text::x() const
{
	return x_;
};

std::string Text::timeToString( int seconds, int minutes, int minutes_padding )
{
	return formatNumDigitPadding( minutes, minutes_padding ) + ":" + formatNumDigitPadding( seconds, 2 );
};

unsigned int Text::testLineLength( const std::string& text, unsigned int line_length, unsigned int letters_so_far )
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