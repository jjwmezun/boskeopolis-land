#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>

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

	constexpr int xOfN( int n, int w )
	{
		return n % w;
	};

	constexpr int yOfN( int n, int w )
	{
		return floor( n / w );
	};

	constexpr int nOfXY( int x, int y, int w )
	{
		return ( y * w ) + x;
	};

	bool nextFrame( int frame, int interval, int duration );

    template<typename T>
    inline T randInCArray( T* array, int count )
    {
        return array[ randInt( count - 1, 0 ) ];
    }

    constexpr int32_t ceil( double num )
    {
        return ( ( double )( ( int )( num ) ) == num )
            ? ( int )( num )
            : ( int )( num ) + ( ( num > 0.0 ) ? 1 : 0 );
    }

    template<typename S>
    inline const S& randEntry( const std::vector<S>& list )
    {
        return list[ randInt( list.size() - 1 ) ];
    };

    inline int randEntry( const std::vector<int>& list )
    {
        return list[ randInt( list.size() - 1 ) ];
    };
};
