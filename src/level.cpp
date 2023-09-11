#include "config.hpp"
#include "json.hpp"
#include "level.hpp"

namespace BSL
{
    void Level::init( Game & game )
    {
        JSON json { "assets/levels/" + slug_ + ".json" };
        JSONArray map_list = json.getArray( "maps" );

        NasrSetGlobalPalette( 1 );
        NasrMoveCamera( 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS );
        map_.init( game, *this );
        sprites_.init( game, map_ );
        inventory_.init( game );
    };

    void Level::update( Game & game, const Controller & controller, float dt )
    {
        sprites_.update( dt, controller, *this, game );
        inventory_.update( dt, *this );
        map_.update( *this, game, dt );
    };

    Tileset & Level::getTileset( std::string name )
    {
        auto it = tilesets_.find( name );
        if ( it == tilesets_.end() )
        {
            Tileset t { name };
            t.init();
            tilesets_.insert( std::pair<std::string, Tileset> ( name, t ) );
            it = tilesets_.find( name );
        }
        return it->second;
    };
}