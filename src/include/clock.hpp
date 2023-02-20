#ifndef CLOCK_H
#define CLOCK_H

#include <cmath>
#include "renderer.hpp"

namespace BSL
{
    class Game;

    class Clock
    {
        public:
            constexpr Clock( unsigned int start_seconds )
            :
                seconds_ ( static_cast<float>( start_seconds % 60 ) ),
                minutes_ ( std::floor( static_cast<float>( start_seconds ) / 60.0f ) ),
                minute_gfx_ ( 0 ),
                second_gfx_ ( 0 )
            {};
            void init( Game & game, ArgList args );
            void addSeconds( float amount );

        private:
            float minutes_;
            float seconds_;
            CounterGraphic minute_gfx_;
            CounterGraphic second_gfx_;
    };
}

#endif // CLOCK_H