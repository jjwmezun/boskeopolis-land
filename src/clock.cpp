#include "clock.hpp"
#include "config.hpp"
#include "game.hpp"

namespace BSL
{
    void Clock::init( Game & game )
    {
        const float y = WINDOW_HEIGHT_PIXELS - 32.0f;
        const float shadow = 0.25f;

        minute_gfx_ = game.render().addCounterGradient
        (
            minutes_,
            1,
            Dir::XY::DOWN,
            64,
            16,
            24.0f,
            y,
            {
                { "shadow", shadow }
            }
        );

        game.render().addTextGradient
        (
            "🕑 :",
            Dir::XY::DOWN,
            64,
            16,
            16.0f,
            y,
            32.0f,
            32.0f,
            {
                { "shadow", shadow }
            }
        );

        second_gfx_ = game.render().addCounterGradient
        (
            seconds_,
            2,
            Dir::XY::DOWN,
            64,
            16,
            40.0f,
            y,
            {
                { "shadow", shadow }
            }
        );
    };

    void Clock::addSeconds( float amount )
    {
        seconds_ += amount;
        while ( seconds_ >= 60.0f )
        {
            minutes_ += 1.0f;
            seconds_ -= 60.0f;
        }

        minute_gfx_.setNumber( minutes_ );
        second_gfx_.setNumber( seconds_ );
    };
}