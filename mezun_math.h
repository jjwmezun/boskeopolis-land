
#ifndef MEZUN_MATH_H
#define MEZUN_MATH_H

#include <algorithm>
#include <cstdlib>

namespace MezunMath
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
};

#endif // MEZUN_MATH_H
