#include <cassert>
#include <cmath>
#include "hex_string.hpp"

static char getHexCharFromDecimalDigit( int n )
{
	assert( n >= 0 && n < 16 );
	if ( n >= 0 && n < 10 )
	{
		return ( char )( 48 + n );
	}
	else if ( n >= 10 && n < 16 )
	{
		return ( char )( 55 + n );
	}
};

static int getHexDigit( int n )
{
	return n % 16;
};

static char getHexCharFromDecimalOnesDigit( int n )
{
	return getHexCharFromDecimalDigit( getHexDigit( n ) );
};

HexString::HexString( int n, int min_digits )
:
	decimal_ ( n ),
	hex_ ( generateHexStringFromDecimal( n, min_digits ) )
{};

const std::string& HexString::getHexString() const
{
	return hex_;
};

int HexString::getDecimal() const
{
	return decimal_;
};

std::string HexString::generateHexStringFromDecimal( int decimal, int min_digits )
{
	std::string hex_string = "";
	hex_string += getHexCharFromDecimalOnesDigit( decimal );
	while ( ( decimal = floor( decimal / 16 ) ) || hex_string.length() < min_digits )
	{
		char c = getHexCharFromDecimalOnesDigit( decimal );
		hex_string.insert( hex_string.begin(), 1, c );
	}
	return hex_string;
};
