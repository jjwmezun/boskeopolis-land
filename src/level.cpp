#include "config.hpp"
#include "level.hpp"

namespace BSL
{
    void Level::init( Game & game )
    {
        NasrSetGlobalPalette( 1 );
        NasrMoveCamera( 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS );
        map_.init( game );
        sprites_.init( game, map_ );
        inventory_.init( game );
    };

    void Level::update( Game & game, const Controller & controller, float dt )
    {
        sprites_.update( dt, controller, *this, game );
        inventory_.update( dt );
        map_.update( *this, game, dt );
    };
}