#include "config.hpp"
#include "level.hpp"

namespace BSL
{
    void Level::init( Game & game )
    {
        NasrSetGlobalPalette( 1 );
        NasrMoveCamera( 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS );
        map_.init( game );
        autumn_.init( game );
        inventory_.init( game );
    };

    void Level::update( Game & game, const Controller & controller, float dt )
    {
        autumn_.update( dt, controller, *this );
        inventory_.update( dt );
    };
}