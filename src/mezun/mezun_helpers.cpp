#include <bitset>
#include <cctype>
#include "mezun_helpers.hpp"
#include <stdexcept>
#include <string>

namespace mezun
{
	static int getCharacterSize( char character );
	static char32_t combineCharactersByCharsize( int charsize, const char* source );

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

	void copyCharToChar32( char32_t* destination, const char* source, int limit )
	{
		int i = 0;
		while ( source[ 0 ] != '\0' && i < limit )
		{
			int charsize = getCharacterSize( source[ 0 ] );
			destination[ i ] = combineCharactersByCharsize( charsize, source );
			source += charsize; // Go forward through the string by charsize.
			++i;
		}
		destination[ i ] = '\0';
	};

	std::u32string charToChar32String( const char* source, int limit )
	{
		char32_t destination[ limit ];
		copyCharToChar32( destination, source, limit );
		return std::u32string( destination );
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
}
