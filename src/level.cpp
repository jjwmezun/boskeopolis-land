#include "config.hpp"
#include "game.hpp"
#include "json.hpp"
#include "level.hpp"
#include "transition_out_message_state.hpp"

namespace BSL
{
    void Level::init( Game & game )
    {
        JSON json { "assets/levels/" + slug_ + ".json" };
        JSONArray map_list = json.getArray( "maps" );
        maps_ = JSONMap<MapData>( map_list, [&]( const JSONItem & i )
        {
            return MapData{ i.asString() };
        });
        for ( auto & map : maps_ )
        {
            map.init( game );
        }
        float entrance_x = 0.0f;
        float entrance_y = 0.0f;
        if ( json.hasFloat( "entrance_x" ) )
        {
            entrance_x = static_cast<float>( blocksToPixels( json.getInt( "entrance_x" ) ) );
        }
        else if ( json.hasInt( "entrance_x" ) )
        {
            entrance_x = static_cast<float>( blocksToPixels( json.getInt( "entrance_x" ) ) );
        }
        if ( json.hasFloat( "entrance_y" ) )
        {
            entrance_y = static_cast<float>( blocksToPixels( json.getInt( "entrance_y" ) ) );
        }
        else if ( json.hasInt( "entrance_y" ) )
        {
            entrance_y = static_cast<float>( blocksToPixels( json.getInt( "entrance_y" ) ) );
        }

        NasrSetGlobalPalette( 1 );
        NasrMoveCamera( 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS );
        current_map_.init( game, *this, maps_[ 0 ] );
        sprites_.init( game, current_map_, entrance_x, entrance_y );
        inventory_.init( game );
    };

    Level::~Level()
    {
        for ( auto & map : maps_ )
        {
            map.clear();
        }
    };

    void Level::update( Game & game, const Controller & controller, float dt )
    {
        sprites_.update( dt, controller, *this, game );
        inventory_.update( dt, *this );
        current_map_.update( *this, game, dt );
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

    void Level::startWarp( Game & game )
    {
        const auto warp = maps_[ current_map_.id() ].getWarp( pos_ );
        if ( warp.has_value() )
        {
            game.pushState( std::make_unique<TransitionOutMessageState> ( 1 ) );
        }
    };

    void Level::doWarp( Game & game, const Controller & controller )
    {
        const auto warp = maps_[ current_map_.id() ].getWarp( pos_ );
        if ( warp.has_value() )
        {
            game.clearGraphics();
            current_map_ = { warp.value().map };
            current_map_.init( game, *this, maps_[ warp.value().map ] );
            sprites_.init( game, current_map_, warp.value().entrance_x, warp.value().entrance_y );
            inventory_.init( game );
            sprites_.update( 0.0f, controller, *this, game );
            inventory_.update( 0.0f, *this );
            current_map_.update( *this, game, 0.0f );
        }
    };
}