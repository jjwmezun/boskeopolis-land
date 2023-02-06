#include "input.hpp"
#include "input_controller.hpp"
#include "nasringine/nasr.h"
#include "sprite.hpp"

static bool running = 1;

int main( int argc, char ** argv )
{
    NasrInit( "Boskeopolis Land", 520, 320, 5, 128, 128, 18, NASR_SAMPLING_NEAREST, NASR_INDEXED_YES, 0, 8 );

    double prev_time = NasrGetTime();
    double current_time = 0;

    NasrSetPalette( "assets/palettes/palette.png" );

    BSL::Input::init();
    BSL::InputController input;

    NasrColor bg { 128.0f, 32.0f, 255.0f, 255.0f };
    NasrGraphicsAddCanvas
    (
        0,
        0,
        0,
        bg
    );

    BSL::Sprite autumn;

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

            autumn.update( dt, input );

            timechange = current_time - prev_time;
            fps = 1.0 / timechange;
            dt = 60.0f / ( float )( fps );
            prev_time = current_time;
            NasrUpdate( dt );
        }
    }
    NasrClose();
}