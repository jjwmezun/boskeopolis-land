#include "config.hpp"
#include "game.hpp"
#include "input.hpp"
#include "title_state.hpp"
#include "nasringine/nasr.h"

static constexpr unsigned int MAX_STATES = 5;

static bool running = 1;

int main( int argc, char ** argv )
{
    NasrInit
    (
        "Boskeopolis Land",
        BSL::WINDOW_WIDTH_PIXELS,
        BSL::WINDOW_HEIGHT_PIXELS,
        MAX_STATES,
        128,
        128,
        18,
        NASR_SAMPLING_NEAREST,
        NASR_INDEXED_YES,
        0,
        8
    );

    double prev_time = NasrGetTime();
    double current_time = 0;

    NasrSetPalette( "assets/palettes/palette.png" );
    BSL::Input::init();

    BSL::Game game { MAX_STATES };
    game.changeState( std::make_unique<BSL::TitleState>() );

    while ( running )
    {
        if ( NasrHasClosed() )
        {
            running = false;
        }
        else
        {
            current_time = NasrGetTime();
            double timechange = current_time - prev_time;
            double fps = 1.0 / timechange;
            float dt = 60.0f / ( float )( fps );

            game.update( dt );

            timechange = current_time - prev_time;
            fps = 1.0 / timechange;
            dt = 60.0f / ( float )( fps );
            prev_time = current_time;
            NasrUpdate( dt );
        }
    }
    NasrClose();
}