#include <cmath>
#include "config.hpp"
#include <cstring>
#include "game.hpp"
#include "map.hpp"
#include "nasringine/json/json.h"
#include "nasringine/nasr.h"
#include "nasringine/nasr_io.h"

#include <iostream>

namespace BSL
{
    enum class MapLayerType
    {
        COLLISION,
        TILE,
        OBJECT
    };

    struct MapLayer
    {
        std::vector<int> tiles;
        unsigned int width;
        unsigned height;
        MapLayerType type;
    };

    Map::Map( std::string && slug )
    :
        slug_ ( slug ),
        width_ ( 0 ),
        height_ ( 0 )
    {};

    void Map::init( Game & game )
    {
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

        // Tiles
        std::string filename = "assets/maps/" + slug_ + ".json";
        char * map_data = NasrReadFile( filename.c_str() );
        if ( !map_data )
        {
            // TODO: Throw exception.
            std::cout << "NO FILE" << std::endl;
        }
        json_char * json = ( json_char * )( map_data );
        json_value * root = json_parse( json, strlen( map_data ) + 1 );
        free( map_data );
        if ( !root || root->type != json_object || !root->u.object.length )
        {
            // TODO: Throw exception.
            std::cout << "NO FILE" << std::endl;
        }

        std::vector<MapLayer> layers;

        for ( unsigned int i = 0; i < root->u.object.length; ++i )
        {
            const json_object_entry root_entry = root->u.object.values[ i ];
            if ( std::strcmp( "width", root_entry.name ) == 0 )
            {
                if ( root_entry.value->type != json_integer )
                {
                    // TODO: Throw exception.
                    std::cout << "NO FILE" << std::endl;
                }

                width_ = root_entry.value->u.integer;
            }
            else if ( std::strcmp( "height", root_entry.name ) == 0 )
            {
                if ( root_entry.value->type != json_integer )
                {
                    // TODO: Throw exception.
                    std::cout << "NO FILE" << std::endl;
                }

                height_ = root_entry.value->u.integer;
            }
            else if ( std::strcmp( "layers", root_entry.name ) == 0 )
            {
                if ( root_entry.value->type != json_array )
                {
                    // TODO: Throw exception.
                    std::cout << "NO FILE" << std::endl;
                }

                for ( unsigned int j = 0; j < root_entry.value->u.array.length; ++j )
                {
                    const json_value * layer_item = root_entry.value->u.array.values[ j ];
                    if ( layer_item->type != json_object )
                    {
                        // TODO: Throw exception.
                        std::cout << "NO FILE" << std::endl;
                    }

                    MapLayer layer;

                    for ( unsigned int k = 0; k < layer_item->u.object.length; ++k )
                    {
                        const json_object_entry layer_entry = layer_item->u.object.values[ k ];
                        if ( std::strcmp( "data", layer_entry.name ) == 0 )
                        {
                            if ( layer_entry.value->type != json_array )
                            {
                                // TODO: Throw exception.
                                std::cout << "NO FILE" << std::endl;
                            }

                            for ( unsigned int l = 0; l < layer_entry.value->u.array.length; ++l )
                            {
                                const json_value * data_item = layer_entry.value->u.array.values[ l ];
                                if ( data_item->type != json_integer )
                                {
                                    // TODO: Throw exception.
                                    std::cout << "NO FILE" << std::endl;
                                }

                                layer.tiles.push_back( data_item->u.integer );
                            }
                        }
                        else if ( std::strcmp( "name", layer_entry.name ) == 0 )
                        {
                            if ( layer_entry.value->type != json_string )
                            {
                                // TODO: Throw exception.
                                std::cout << "NO FILE" << std::endl;
                            }

                            if ( std::strcmp( "collision", layer_entry.value->u.string.ptr ) == 0 )
                            {
                                layer.type = MapLayerType::COLLISION;
                            }
                            else if ( std::strcmp( "tile", layer_entry.value->u.string.ptr ) == 0 )
                            {
                                layer.type = MapLayerType::TILE;
                            }
                            else if ( std::strcmp( "object", layer_entry.value->u.string.ptr ) == 0 )
                            {
                                layer.type = MapLayerType::OBJECT;
                            }
                            else
                            {
                                // TODO: Throw exception.
                                std::cout << "NO FILE" << std::endl;
                            }
                        }
                    }

                    layers.push_back( layer );
                }
            }
        }
        json_value_free( root );

        for ( const MapLayer & layer : layers )
        {
            switch ( layer.type )
            {
                case ( MapLayerType::COLLISION ):
                {
                    collision_.push_back( layer.tiles );
                }
                break;
                case ( MapLayerType::TILE ):
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
                                i % 16,
                                std::floor( i / 16 ),
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
                case ( MapLayerType::OBJECT ):
                {
                    for ( int tile : layer.tiles )
                    {
                        if ( tile == 0 )
                        {
                            continue;
                        }

                        int objtype = tile - 1121;
                        if ( objtype < 0 )
                        {
                            printf( "Invalid object.\n" );
                        }
                        else
                        {
                        }
                    }
                }
                break;
            }
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