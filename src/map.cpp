#include <cmath>
#include "config.hpp"
#include "game.hpp"
#include "json.hpp"
#include "map.hpp"
#include "map_layer_palette_change.hpp"
#include "map_layer_rain.hpp"
#include "nasringine/nasr.h"
#include <unordered_map>

#include "tileset.hpp"

#include <iostream>

namespace BSL
{
    struct MapTileLayer
    {
        enum class Type
        {
            COLLISION,
            TILE,
            OBJECT
        };

        std::vector<int> tiles;
        MapTileLayer::Type type;
    };

    Map::Map( std::string && slug )
    :
        slug_ ( slug ),
        width_ ( 0 ),
        height_ ( 0 )
    {};

    void Map::init( Game & game )
    {
        // Init objects tileset.
        Tileset objects { "objects" };
        objects.init();

        // Backgrounds.
        game.render().addRectGradient
        (
            0.0f,
            0.0f,
            BSL::WINDOW_WIDTH_PIXELS,
            BSL::WINDOW_HEIGHT_PIXELS,
            Dir::XY::DOWN,
            1,
            254,
            true
        );
        layers_.emplace_back( std::make_unique<MapLayerRain>( 32, 128 ) );
        layers_.emplace_back( std::make_unique<MapLayerPaletteChange>( 32, 128 ) );
        for ( auto & layer : layers_ )
        {
            layer->init( game );
        }

        // Get tile data from JSON file.
        JSON json { "assets/maps/" + slug_ + ".json" };
        width_ = json.getInt( "width" );
        height_ = json.getInt( "height" );
        JSONArray l = json.getArray( "layers" );
        std::vector<MapTileLayer> layers = JSONMap<MapTileLayer>
        (
            l,
            [&]( const JSONItem & i )
            {
                JSONObject o = i.asObject();

                JSONArray tiles = o.getArray( "data" );

                std::string name = o.getString( "name" );
                std::unordered_map<std::string, MapTileLayer::Type> t
                {
                    { "collision", MapTileLayer::Type::COLLISION },
                    { "tile", MapTileLayer::Type::TILE },
                    { "object", MapTileLayer::Type::OBJECT }
                };
                auto it = t.find( name );
                if ( it == t.end() )
                {
                    throw std::runtime_error( "Map " + slug_ + " has invalid layer type " + name );
                }

                return MapTileLayer
                {
                    JSONMap<int>
                    (
                        tiles,
                        []( const JSONItem & di )
                        {
                            return di.asInt();
                        }
                    ),
                    it->second
                };
            }   
        );

        // Generate map data from tile data.
        for ( const MapTileLayer & layer : layers )
        {
            switch ( layer.type )
            {
                case ( MapTileLayer::Type::COLLISION ):
                {
                    collision_.push_back( layer.tiles );
                }
                break;
                case ( MapTileLayer::Type::TILE ):
                {
                    std::vector<NasrTile> tiles;
                    for ( int tile : layer.tiles )
                    {
                        NasrTile t;
                        if ( tile == 0 )
                        {
                            t = { 0, 0, 0, 255 };
                        }
                        else
                        {
                            const int i = tile - 7073;
                            t =
                            {
                                static_cast<unsigned char>( i % 16 ),
                                static_cast<unsigned char>( std::floor( i / 16 ) ),
                                0,
                                0
                            };
                        }

                        tiles.push_back( t );
                    }

                    game.render().addTilemap
                    (
                        "urban",
                        tiles,
                        width_,
                        height_
                    );
                }
                break;
                case ( MapTileLayer::Type::OBJECT ):
                {
                    blocks_.push_back({});
                    std::vector<NasrTile> tiles;
                    for ( int tile : layer.tiles )
                    {
                        NasrTile t { 0, 0, 0, 255 };
                        const BlockType * type = nullptr;
                        if ( tile > 0 )
                        {
                            int objtype = tile - 1121;
                            if ( objtype < 0 )
                            {
                                printf( "Invalid object.\n" );
                            }
                            else
                            {
                                type = objects.getBlockType( objtype );
                                if ( type )
                                {
                                    t =
                                    {
                                        type->getX(),
                                        type->getY(),
                                        0,
                                        type->getAnimation()
                                    };
                                }
                            }
                        }
                        blocks_[ blocks_.size() - 1 ].push_back( type );
                        tiles.push_back( t );
                    }
                    block_layers_.push_back
                    (
                        game.render().addTilemap
                        (
                            "objects",
                            tiles,
                            width_,
                            height_
                        )
                    );
                }
                break;
            }
        }
    };

    void Map::update( Level & level, const Game & game, float dt )
    {
        for ( auto & layer : layers_ )
        {
            layer->update( level, game, dt );
        }
    };

    bool Map::testCollision( unsigned int x, unsigned int y, std::vector<CollisionType> types ) const
    {
        if ( x > width_ || y > height_ ) return false;

        const int i = getIFromXAndY( x, y );
        for ( const std::vector<int> & layer : collision_ )
        {
            for ( const CollisionType type : types )
            {
                if ( layer[ i ] == static_cast<int>( type ) )
                {
                    return true;
                }
            }
        }

        return false;
    };

    unsigned int Map::getWidthPixels() const { return blocksToPixels( width_ ); };
    unsigned int Map::getHeightPixels() const { return blocksToPixels( height_ ); };
}