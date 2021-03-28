#include "map_layer_rain.hpp"
#include "mezun_math.hpp"
#include "render.hpp"

static int randomColor();
static int randomSpeed();
static int getMapIFromX( int x );

MapLayerRain::MapLayerRain( Unit::Layer layer_position, int number_of_drops )
:
    MapLayer ( layer_position ),
    number_of_drops_ ( number_of_drops )
{
    // Init all rain map quadrants to false & all drops.
    for ( int i = 0; i < MAX_DROPS; ++i )
    {
        rain_map_[ i ] = false;
    }
    for ( int i = 0; i < number_of_drops_; ++i )
    {
        generateDrop( i );
    }
};

MapLayerRain::~MapLayerRain() {};

void MapLayerRain::update( LevelState& level_state )
{
    for ( int i = 0; i < number_of_drops_; ++i )
    {
        // If drop is past bottom o’ screen, regenerate.
        if ( drops_[ i ].rect.y > Unit::WINDOW_HEIGHT_PIXELS )
        {
            // Since this is new, make sure we release rain map entry.
            removeDrop( i );
            generateDrop( i );
        }
        else
        {
            // Move drop downward by speed.
            drops_[ i ].rect.y += drops_[ i ].speed;
        }
    }
};

void MapLayerRain::render( const Camera& camera )
{
    for ( int i = 0; i < number_of_drops_; ++i )
    {
        Render::renderRect( drops_[ i ].rect, drops_[ i ].color, 128 );
    }
};

void MapLayerRain::generateDrop( int i )
{
    int x;
    int rain_map_index;

    // Generate random x position from left to right side o’ screen
    // & keep generating till we get 1 from an unused quadrant,
    // & then when found, set that that quadrant is used.
    // Quadrants are 2-pixel-wide columns that span from the left side
    // o’ the screen to the right.
    // We map them to ensure regularly-distributed rain.
    // Since there are 200 entries in the rain map &
    // max 200 rain drops, this should ne’er be an infinite loop.
    do
    {
        x = mezun::randInt( Unit::WINDOW_WIDTH_PIXELS, 0 );
        rain_map_index = getMapIFromX( x );
    }
    while ( rain_map_[ rain_map_index ] );
    rain_map_[ rain_map_index ] = true;

    const int height = mezun::randInt( 64, 4 );
    drops_[ i ] =
    {
        { x, -height, 1, height },
        randomColor(),
        randomSpeed()
    };
};

void MapLayerRain::removeDrop( int i )
{
    rain_map_[ getMapIFromX( drops_[ i ].rect.x ) ] = false;
};

static int randomColor()
{
    return mezun::randInt( 5, 1 );
};

static int randomSpeed()
{
    return mezun::randInt( 6, 2 );
};

static int getMapIFromX( int x )
{
    return ( int )( std::floor( ( double )( x ) / ( ( double )( Unit::WINDOW_WIDTH_PIXELS ) / ( double )( MapLayerRain::MAX_DROPS ) ) ) );
};