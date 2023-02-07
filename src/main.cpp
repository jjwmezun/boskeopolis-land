#include "config.hpp"
#include "input.hpp"
#include "input_controller.hpp"
#include "map.hpp"
#include "nasringine/nasr.h"
#include "sprite.hpp"

static bool running = 1;

int main( int argc, char ** argv )
{
    NasrInit( "Boskeopolis Land", BSL::WINDOW_WIDTH_PIXELS, BSL::WINDOW_HEIGHT_PIXELS, 5, 128, 128, 18, NASR_SAMPLING_NEAREST, NASR_INDEXED_YES, 0, 8 );

    double prev_time = NasrGetTime();
    double current_time = 0;

    NasrSetPalette( "assets/palettes/palette.png" );
    NasrSetGlobalPalette( 1 );
    NasrMoveCamera( 0, 0, BSL::WINDOW_WIDTH_PIXELS, BSL::WINDOW_HEIGHT_PIXELS );

    BSL::Input::init();
    BSL::InputController input;

    NasrColor bg { 128.0f, 32.0f, 255.0f, 255.0f };
    NasrRect r { 0.0f, 0.0f, BSL::WINDOW_WIDTH_PIXELS, BSL::WINDOW_HEIGHT_PIXELS };
    NasrGraphicsAddRectGradientPalette
    (
        1,
        0,
        0,
        r,
        0,
        NASR_DIR_DOWN,
        1,
        254,
        1
    );

    BSL::Map map { "city-1" };
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

            autumn.update( dt, input, map );

            timechange = current_time - prev_time;
            fps = 1.0 / timechange;
            dt = 60.0f / ( float )( fps );
            prev_time = current_time;
            NasrUpdate( dt );
        }
    }
    NasrClose();
}