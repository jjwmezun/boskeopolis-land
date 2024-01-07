#include "config.hpp"
#include "controls.hpp"
#include "game.hpp"
#include "gfx.hpp"

#include <cstdio>

static bool running = true;
static double fixed_timestep = 0.0f;
static double fixed_timestep_update = 1.0f / fixed_timestep;
float total_dt = 0.0f;
unsigned int dtcount = 0;

int main( int argc, char ** argv )
{
    BSL::GFX::init();
    BSL::Controls::init();
    BSL::Game::init();

    double prev_time = BSL::GFX::getTime();
    double accumulated_time = prev_time;

    while ( running )
    {
        if ( BSL::GFX::testHasClosed() )
        {
            running = false;
        }
        else
        {
            BSL::GFX::handleEvents();
            double current_time = BSL::GFX::getTime();
            double timechange = current_time - prev_time;
            double fps = 1.0 / timechange;
            float dt = 60.0f / ( float )( fps );

            total_dt += ( 60.0f / dt );
            ++dtcount;

            BSL::Game::update( dt );
            BSL::GFX::update( dt );
            BSL::Controls::update();
            prev_time = current_time;
        }
    }

    printf( "%f\n", total_dt / dtcount );
    BSL::GFX::close();

    return 0;
};