#include <cstdlib>
#include <ctime>
#include "math.hpp"

namespace BSL
{
    namespace Math
    {
        std::default_random_engine rng_;

        void init()
        {
            // Seed randomizer.
            srand( time( 0 ) );

            // Set up RNG.
            auto rd = std::random_device {};
            rng_ = std::default_random_engine{ rd() };
        };

        float randInt( int max, int min )
        {
            return rand() % ( max - min + 1 ) + min;
        };

        std::default_random_engine getRNG()
        {
            return rng_;
        };
    }
}