#include <cmath>
#include "config.hpp"
#include "controls.hpp"
#include "dict.hpp"
#include "gfx.hpp"
#include "json.hpp"
#include "level.hpp"
#include "level_table.hpp"
#include "object_factory.hpp"
#include <regex>
#include <stdexcept>
#include <string>

namespace BSL
{
    static constexpr float CAMERA_RIGHT_EDGE = WINDOW_WIDTH_PIXELS * 0.667f;
    static constexpr float CAMERA_LEFT_EDGE = WINDOW_WIDTH_PIXELS * 0.333f;
    static constexpr float CAMERA_BOTTOM_EDGE = WINDOW_HEIGHT_PIXELS * 0.667f;
    static constexpr float CAMERA_TOP_EDGE = WINDOW_HEIGHT_PIXELS * 0.333f;

    void Level::init( uint_fast8_t levelid )
    {
        // Init map.
        const std::string mapslug = level_table[ levelid ].maps[ 0 ]; // Using 1st map.
        const JSON json { "assets/maps/" + mapslug + ".json" };

        // Get width & height.
        map.w = json.getInt( "width" );
        map.h = json.getInt( "height" );

        // Init collision table.
        collision = static_cast<uint_fast8_t *> ( calloc( map.w * map.h, sizeof( uint_fast8_t ) ) );

        // Generate empty list o’ tiles for objects.
        BSL::GFX::Tile object_tilemap[ map.w * map.h ];
        memset( object_tilemap, 0, map.w * map.h * sizeof( BSL::GFX::Tile ) );

        // Init empty objects table.
        objects = static_cast<Object *> ( calloc( map.w * map.h, sizeof( Object ) ) );

        // Init sprites list.
        sprites.init( 16 );

        // Get layer data.
        const JSONArray layers = json.getArray( "layers" );
        layers.forEach
        (
            [&] ( const JSONItem item )
            {
                const JSONObject layerobj = item.asObject();
                const JSONArray layerprops = layerobj.getArray( "properties" );

                // Generate hash map from layer properties.
                Dict<JSONItem> props;
                layerprops.forEach
                (
                    [&] ( const JSONItem propitem )
                    {
                        const JSONObject propobj = propitem.asObject();
                        const std::string propname = propobj.getString( "name" );
                        const JSONItem propvalue = propobj.getItem( "value" );
                        props.add( propname, propvalue );
                    }
                );

                const std::string typestring = props.get( "type" ).asString();
                if ( typestring == "tile" )
                {
                    const JSONArray tiledata = layerobj.getArray( "data" );
                    if ( tiledata.getLength() != map.w * map.h )
                    {
                        throw std::runtime_error( "Map “" + mapslug + "” tile layer poorly formed: size o’ tiles different from map height x map width." );
                    }
                    BSL::GFX::Tile universal_tilemap[ tiledata.getLength() ];
                    BSL::GFX::Tile specific_tilemap[ tiledata.getLength() ];

                    // Zero out tilemaps so they don’t keep tiles from previous loops or have junk data.
                    const size_t tilemapsize = tiledata.getLength() * sizeof( BSL::GFX::Tile );
                    memset( universal_tilemap, 0x00, tilemapsize );
                    memset( specific_tilemap, 0x00, tilemapsize );

                    bool uses_universal = false;
                    bool uses_specific_tileset = false;
                    tiledata.forEach
                    (
                        [ & ]( const JSONItem dataitem, uint_fast16_t i )
                        {
                            const uint_fast16_t tilen = dataitem.asInt();

                            if ( tilen < 24577 && tilen > 0 )
                            {
                                throw std::runtime_error( "Invalid tile # “" + std::to_string( tilen ) + "” used for tile layer on map “" + mapslug + "”." );
                            }

                            if ( tilen == 0 )
                            {
                                return;
                            }

                            const bool notuniversal = tilen > 32768;
                            const uint_fast16_t realtilen = tilen - ( notuniversal ? 32769 : 24577 );

                            BSL::GFX::Tile * tilemap = notuniversal ? specific_tilemap : universal_tilemap;
                            tilemap[ i ].set = tilen > 24576;
                            tilemap[ i ].x = realtilen % 16;
                            tilemap[ i ].y = static_cast<uint_fast8_t> ( realtilen / 16.0 );
                            tilemap[ i ].animation = 0;
                            tilemap[ i ].frame = 0;

                            if ( tilemap[ i ].set )
                            {
                                if ( notuniversal )
                                {
                                    uses_specific_tileset = true;
                                }
                                else
                                {
                                    uses_universal = true;
                                }
                            }

                            ++i;
                        }
                    );

                    float scrollx = 1.0f;
                    float scrolly = 1.0f;
                    int offsetx = 0;
                    int offsety = 0;
                    if ( layerobj.hasFloat( "parallaxx" ) )
                    {
                        scrollx = layerobj.getFloat( "parallaxx" );
                    }
                    if ( layerobj.hasFloat( "parallaxy" ) )
                    {
                        scrollx = layerobj.getFloat( "parallaxy" );
                    }

                    if ( uses_universal )
                    {
                        const uint_fast16_t universal_texture = BSL::GFX::loadFileAsTexture( "tilesets/universal.png" );
                        BSL::GFX::addGraphicTilemap
                        (
                            universal_texture,
                            universal_tilemap,
                            map.w,
                            map.h,
                            {
                                { "scrollx", scrollx },
                                { "scrolly", scrolly },
                                { "offsetx", offsetx },
                                { "offsety", offsety },
                            }
                        );
                    }

                    if ( uses_specific_tileset )
                    {
                        const uint_fast16_t specific_texture = BSL::GFX::loadFileAsTexture( "tilesets/urban.png" );
                        BSL::GFX::addGraphicTilemap
                        (
                            specific_texture,
                            specific_tilemap,
                            map.w,
                            map.h,
                            {
                                { "scrollx", scrollx },
                                { "scrolly", scrolly },
                                { "offsetx", offsetx },
                                { "offsety", offsety },
                            }
                        );
                    }
                }
                else if ( typestring == "object" )
                {
                    // Get list o’ tile indices.
                    const JSONArray tile_index_list = layerobj.getArray( "data" );

                    // Make sure tile index list length fills whole map.
                    if ( tile_index_list.getLength() != map.w * map.h )
                    {
                        throw std::runtime_error( "Map “" + mapslug + "” tile layer poorly formed: size o’ tiles different from map height x map width." );
                    }

                    // Go thru each tile index, check if there is a valid object,
                    // & generate graphics & object from valid object IDs.
                    tile_index_list.forEach
                    (
                        [ & ]( const JSONItem dataitem, uint_fast16_t i )
                        {
                            // Get index #.
                            const uint_fast16_t tilen = dataitem.asInt();

                            // Throw error if non-object included in object layer.
                            if ( tilen > 0 && tilen < 8193 || tilen > 16384 )
                            {
                                throw std::runtime_error( "Object ID “" + std::to_string( tilen ) + "” out o’ range." );
                            }

                            // Skip if it’s an empty tile.
                            if ( tilen == 0 )
                            {
                                return;
                            }

                            const uint_fast16_t realtilen = tilen - 8193;
                            const auto objtemp_container = ObjectFactory::get( realtilen );

                            // Throw error if object template doesn’t exist.
                            if ( !objtemp_container.has_value() )
                            {
                                throw std::runtime_error( "Invalid object #" + std::to_string( realtilen ) + " ( tile index " + std::to_string( tilen ) + " ) found in map." );
                            }

                            const auto objtemp = objtemp_container.value();

                            // Add graphics info to tile.
                            object_tilemap[ i ].set = true;
                            object_tilemap[ i ].x = objtemp.tile.x;
                            object_tilemap[ i ].y = objtemp.tile.y;
                            object_tilemap[ i ].animation = objtemp.tile.animation;

                            // Add object.
                            objects[ i ].type = objtemp.type;
                            objects[ i ].data = objtemp.data;
                        }
                    );
                }
                else if ( typestring == "collision" )
                {
                    const JSONArray tiledata = layerobj.getArray( "data" );
                    if ( tiledata.getLength() != map.w * map.h )
                    {
                        throw std::runtime_error( "Map “" + mapslug + "” tile layer poorly formed: size o’ tiles different from map height x map width." );
                    }
                    tiledata.forEach
                    (
                        [ & ]( const JSONItem dataitem, uint_fast16_t i )
                        {
                            const uint_fast8_t tilen = dataitem.asInt();
                            if ( tilen > 0 )
                            {
                                collision[ i ] = tilen;
                            }
                        }
                    );
                }
                else if ( typestring == "sprite" )
                {
                    const JSONArray indices = layerobj.getArray( "data" );
                    if ( indices.getLength() != map.w * map.h )
                    {
                        throw std::runtime_error( "Map “" + mapslug + "” tile layer poorly formed: size o’ tiles different from map height x map width." );
                    }
                    indices.forEach
                    (
                        [ & ]( const JSONItem dataitem, uint_fast16_t i )
                        {
                            const uint_fast16_t index = dataitem.asInt();

                            // Skip blank indices.
                            if ( index == 0 )
                            {
                                return;
                            }

                            const uint_fast16_t x = i % map.w;
                            const uint_fast16_t y = std::floor( i / map.w );
                            const uint_fast16_t relative_index = index - 16384;
                            if ( relative_index == 1 )
                            {
                                player.x = x * BLOCK_SIZE;
                                player.y = y * BLOCK_SIZE;
                            }
                            else
                            {
                                Sprite & sprite = sprites.pushEmptyGet();
                                sprite.init( relative_index, x, y );
                            }
                        }
                    );
                }
                else if ( typestring == "image" )
                {
                    const std::string src = std::regex_replace( layerobj.getString( "image" ), std::regex( "../graphics/" ), "" );
                    const uint_fast16_t texture = BSL::GFX::loadFileAsTexture( src.c_str() );
                    const bool repeatx = layerobj.getBoolOptional( "repeatx", false );
                    const bool repeaty = layerobj.getBoolOptional( "repeaty", false );
                    const float scrollx = layerobj.getFloatOptional( "parallaxx", 1.0f );
                    const float scrolly = layerobj.getFloatOptional( "parallaxy", 1.0f );
                    const int offsetx = layerobj.getIntOptional( "offsetx", 0 );
                    const int offsety = layerobj.getIntOptional( "offsety", 0 );
                    BSL::GFX::addGraphicWallpaper
                    (
                        texture,
                        {
                            { "repeatx", repeatx },
                            { "repeaty", repeaty },
                            { "scrollx", scrollx },
                            { "scrolly", scrolly },
                            { "offsetx", offsetx },
                            { "offsety", offsety }
                        }
                    );
                }
                // TODO: all other types & throw exception on encountering unfamiliar type.
            }
        );

        // Create graphic layer for objects.
        const uint_fast16_t objects_texture = BSL::GFX::loadFileAsTexture( "tilesets/objects.png" );
        object_gfx = BSL::GFX::addGraphicTilemap
        (
            objects_texture,
            object_tilemap,
            map.w,
            map.h
        );

        // Init player.
        player.init();

        // Init inventory.
        inventory.init();
    }

    void Level::update( float dt )
    {
        player.update( dt, *this );

        // Update camera.
        if ( player.getCenterX() > camera.x + CAMERA_RIGHT_EDGE )
        {
            camera.x = player.getCenterX() - CAMERA_RIGHT_EDGE;
            if ( camera.x > map.w * BLOCK_SIZE - WINDOW_WIDTH_PIXELS )
            {
                camera.x = map.w * BLOCK_SIZE - WINDOW_WIDTH_PIXELS;
            }
        }
        else if ( player.getCenterX() < camera.x + CAMERA_LEFT_EDGE )
        {
            camera.x = player.getCenterX() - CAMERA_LEFT_EDGE;
            if ( camera.x < 0.0f )
            {
                camera.x = 0.0f;
            }
        }
        if ( player.getCenterY() > camera.y + CAMERA_BOTTOM_EDGE )
        {
            camera.y = player.getCenterY() - CAMERA_BOTTOM_EDGE;
            if ( camera.y > map.h * BLOCK_SIZE - WINDOW_HEIGHT_PIXELS )
            {
                camera.y = map.h * BLOCK_SIZE - WINDOW_HEIGHT_PIXELS;
            }
        }
        else if ( player.getCenterY() < camera.y + CAMERA_TOP_EDGE )
        {
            camera.y = player.getCenterY() - CAMERA_TOP_EDGE;
            if ( camera.y < 0.0f )
            {
                camera.y = 0.0f;
            }
        }
        BSL::GFX::setCameraX( static_cast<uint_fast32_t> ( camera.x ) );
        BSL::GFX::setCameraY( static_cast<uint_fast32_t> ( camera.y ) );


        // Update sprites.
        for ( uint_fast16_t i = 0; i < sprites.count; ++i )
        {
            sprites.data[ i ].update( dt, *this );
        }


        // Update inventory.
        inventory.update( dt );
    };
};