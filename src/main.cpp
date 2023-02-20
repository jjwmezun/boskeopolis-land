#include "config.hpp"
#include "game.hpp"
#include "input.hpp"
#include "title_state.hpp"
#include "nasringine/nasr.h"
#include "nasringine/nasr_input.h"
#include "nasringine/nasr_localization.h"

static constexpr unsigned int MAX_STATES = 5;

static bool running = 1;
static double fixed_timestep = 0.0f;
static double fixed_timestep_update = 1.0f / fixed_timestep;

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
        1,
        8
    );

    double prev_time = NasrGetTime();
    double accumulated_time = prev_time;

    NasrSetLanguage( "assets/localization/es.json", "boskeopolis-land" );
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
            NasrHandleEvents();
            if ( fixed_timestep )
            {
                double current_time = NasrGetTime();
                double timechange = current_time - prev_time;
                accumulated_time += timechange;
                while ( accumulated_time >= fixed_timestep_update )
                {
                    accumulated_time -= fixed_timestep_update;
                    game.update( 60.0f / static_cast<float>( fixed_timestep ) );
                }
                NasrUpdate( 1.0f );
                prev_time = current_time;
            }
            else
            {
                double current_time = NasrGetTime();
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
            NasrInputUpdate();
        }
    }
    NasrClose();
}