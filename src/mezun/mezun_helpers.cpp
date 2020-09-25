#include <bitset>
#include <cctype>
#include "mezun_helpers.hpp"
#include <regex>
#include <stdexcept>
#include <string>

namespace mezun
{
	static int getCharacterSize( char character );
	static char32_t combineCharactersByCharsize( int charsize, const char* source );
	static std::vector<int> breakIntegerIntoDigitList( int n );
	static std::u32string addDigitsToString( std::u32string text, const std::vector<int>& digits );

	// Combine charsize # o’ 8-bit #s into single 32-bit #.
	template<int charsize>
	static char32_t combineCharacters( const char* source )
	{
		std::bitset<8> binary_pieces[ charsize ];
		for ( int i = 0; i < charsize; ++i )
		{
			binary_pieces[ i ] = ( source[ i ] );
		}
		std::bitset<8 * charsize> binary_full;
		for ( int j = 0; j < 8; ++j )
		{
			for ( int k = 0; k < charsize; ++k )
			{
				binary_full[ j + ( 8 * k ) ] = binary_pieces[ charsize - k - 1 ][ j ];
			}
		}
		return ( char32_t )( binary_full.to_ulong() );
	}

	std::string addLine( const std::string& string, int indent_level )
	{
		std::string text = "";
		for ( int i = 0; i < indent_level; ++i )
		{
			text += "\t";
		}
		return text + string + "\n";
	}

	bool isCStringOnlyNumeric( const char* c_string )
	{
		while ( *c_string )
		{
			if ( !isdigit( *c_string ) )
			{
				return false;
			}
			++c_string;
		}
		return true;
	};

	char32_t copyCharToChar32( const char* source )
	{
		int charsize = getCharacterSize( source[ 0 ] );
		return combineCharactersByCharsize( charsize, source );
	};

	std::u32string charToChar32String( const char* source )
	{
		std::u32string text;
		while ( source[ 0 ] != '\0' )
		{
			int charsize = getCharacterSize( source[ 0 ] );
			text += combineCharactersByCharsize( charsize, source );
			source += charsize;
		}
		//text += ( char32_t )( '\0' );
		return text;
	};

    static int getCharacterSize( char character )
    {
		// See https://en.wikipedia.org/wiki/UTF-8#Description for rules regarding
		// bits set for # o’ bytes used.
        std::bitset<8> binary( character );
        return  ( binary.test( 7 ) )
                ?   ( binary.test( 5 ) )
                    ?   ( binary.test( 4 ) )
                        ? 4
                        : 3
                    : 2
                : 1;
    }

	static char32_t combineCharactersByCharsize( int charsize, const char* source )
	{
		switch ( charsize )
		{
			case ( 1 ):
			{
				return combineCharacters<1>( source );
			}
			break;

			case ( 2 ):
			{
				return combineCharacters<2>( source );
			}
			break;

			case ( 3 ):
			{
				return combineCharacters<3>( source );
			}
			break;

			case ( 4 ):
			{
				return combineCharacters<4>( source );
			}
			break;

			default:
			{
				throw std::runtime_error( "Trying to convert unsupported UTF-8 character to UTF-32." );
			}
			break;
		}
	}

	std::u32string intToChar32String( int n )
	{
		std::u32string text = ( n < 0 ) ? U"-" : U"";
		const auto digits = breakIntegerIntoDigitList( n );
		return addDigitsToString( text, digits );
	};

	std::u32string intToChar32StringWithPadding( int n, int padding )
	{
		std::u32string text = ( n < 0 ) ? U"-" : U"";
		const auto digits = breakIntegerIntoDigitList( n );
		while ( padding > ( int )( digits.size() ) )
		{
			text += ( char32_t )( 48 );
			--padding;
		}
		return addDigitsToString( text, digits );
	};

	std::string string32ToString8( const std::u32string source )
	{
		std::string output = "";
		for ( char32_t character : source )
		{
			output += ( char )( character );
		}
		return output;
	};

	std::string stringReplace( const std::string& in, std::string from, std::string to )
	{
		return std::regex_replace( in, std::regex( from ), to );
	};

	std::u32string removeEndingZeroFrom32String( const std::u32string& string )
	{
		return ( ( int )( string[ string.length() - 1 ] ) == 0 )
			? string.substr( 0, string.length() - 1 )
			: string;
	};

	static std::vector<int> breakIntegerIntoDigitList( int n )
	{
		if ( n == 0 )
		{
			return { 0 };
		}

		n = std::abs( n );
		std::vector<int> digits;
		while ( n > 0 )
		{
			digits.emplace_back( n % 10 );
			n /= 10;
		}
		std::reverse( digits.begin(), digits.end() );
		return digits;
	};

	static std::u32string addDigitsToString( std::u32string text, const std::vector<int>& digits )
	{
		for ( int d : digits )
		{
			text += ( char32_t )( 48 + d );
		}
		return text;
	};

	std::u32string merge32Strings( const std::u32string& first, const std::u32string& second )
	{
		std::u32string out = U"";
		for ( char32_t c : first )
		{
			if ( c == ( char32_t )( 0 ) )
			{
				break;
			}
			out += c;
		}
		for ( char32_t c : second )
		{
			if ( c == ( char32_t )( 0 ) )
			{
				break;
			}
			out += c;
		}
		//out += ( char32_t )( 0 );
		return out;
	};

	bool stringStartsWith( const std::string& haystack, const std::string& needle )
	{
		return haystack.rfind( needle, 0 ) == 0;
	};

	bool stringEndsWith( const std::string& haystack, const std::string& needle )
	{
	    return ( haystack.length() >= needle.length() )
			? ( 0 == haystack.compare( haystack.length() - needle.length(), needle.length(), needle ) )
			: false;
	};

	std::u32string stringReplace( std::u32string target, const std::u32string& from, const std::u32string& to )
	{
		const size_t start_position = target.find( from );
		if ( start_position == std::u32string::npos )
		{
			return target;
		}
		target.replace( start_position, from.length(), to );
		return target;
	};
}
