#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

namespace mezun
{
    constexpr double PI = 3.14159;
    constexpr double DOUBLE_PI = PI * 2;
    constexpr double HALF_PI = PI / 2;

    constexpr double convertDegreesToRadians( double degrees )
    {
        return ( PI / 180.0 ) * degrees;
    };

    constexpr double convertRadiansToDegrees( double radians )
    {
        return radians * ( 180.0 / PI );
    };

    inline void initRand()
    {
        srand ( time( nullptr ) );
    };

    inline int randInt( int max_value, int min_value = 0 )
    {
        // STD Min & Max makes this safe for value mixups.
        return rand() % ( std::max( max_value, min_value ) + 1 - std::min( min_value, max_value ) ) + std::min( min_value, max_value );
    };

	inline bool randBool()
	{
		return ( bool )( randInt( 1, 0 ) );
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

	bool nextFrame( int frame, int interval, int duration );
};
