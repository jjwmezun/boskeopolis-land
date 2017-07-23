#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace mezun
{
    inline int randInt( int max_value, int min_value = 0 )
    {
        // STD Min & Max makes this safe for value mixups.
        return rand() % ( std::max( max_value, min_value ) + 1 - std::min( min_value, max_value ) ) + std::min( min_value, max_value );
    };

    inline int randPercent()
    {
        return randInt( 100, 1 );
    };

    inline bool testRandomWithinPercentage( int percent_interval )
    {
        // The greater the percent_interval, the greater the probability.
        // 100 or mo' always true; 0 or less always false.
        return randPercent() <= percent_interval;
    };
	
	inline bool isAtRightAngle( int value )
	{
		return value % 90 == 0;
	};
	
	static constexpr int xOfN( int n, int w )
	{
		return n % w;
	};
	
	static constexpr int yOfN( int n, int w )
	{
		return floor( n / w );
	};
	
	static constexpr int nOfXY( int x, int y, int w )
	{
		return ( y * w ) + x;
	};
};