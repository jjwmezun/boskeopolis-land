#include "clock.hpp"
#include "config.hpp"
#include "game.hpp"

namespace BSL
{
    void Clock::init( Game & game, ArgList args )
    {
        float x = GetArg( "x", args, 0.0f );
        float y = GetArg( "y", args, 0.0f );
        Dir::XY dir = GetArg( "dir", args, Dir::XY::DOWN );

        args[ "text" ] = "🕑";
        args[ "w" ] = 9.0f;
        args[ "h" ] = 9.0f;
        game.render().addText( args );

        args[ "num" ] = minutes_;
        args[ "maxdigits" ] = 1;
        args[ "x" ] = x + 8.0f;

        minute_gfx_ = game.render().addCounter( args );

        args[ "text" ] = ":";
        args[ "x" ] = x + 16.0f;
        game.render().addText( args );

        args[ "num" ] = seconds_;
        args[ "maxdigits" ] = 2;
        args[ "x" ] = x + 24.0f;

        second_gfx_ = game.render().addCounter( args );
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