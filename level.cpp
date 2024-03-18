#include <cmath>
#include "config.hpp"
#include "controls.hpp"
#include "dict.hpp"
#include "game.hpp"
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

    static void continueWarp();

    void Level::init( uint_fast8_t levelid )
    {
        // Init maps.
        mapcount = level_table[ levelid ].maps.size();
        maps = static_cast<Map *> ( calloc( mapcount, sizeof( Map ) ) );
        for ( uint_fast8_t i = 0; i < level_table[ levelid ].maps.size(); ++i )
        {
            Map & map = maps[ i ];
            const std::string & mapslug = level_table[ levelid ].maps[ i ];
            const JSON json { "assets/maps/" + mapslug + ".json" };

            // Get width & height.
            map.width = json.getInt( "width" );
            map.height = json.getInt( "height" );

            // Init collision table.
            map.collision = static_cast<uint_fast8_t *> ( calloc( map.width * map.height, sizeof( uint_fast8_t ) ) );

            // Init sprites.
            map.sprites = static_cast<uint_fast16_t *> ( calloc( map.width * map.height, sizeof( uint_fast16_t ) ) );

            // Init objects.
            map.objects = static_cast<uint_fast16_t *> ( calloc( map.width * map.height, sizeof( uint_fast16_t ) ) );

            // Init tiles.
            map.tiles.init( 2 );

            // Init images.
            map.images.init( 2 );

            // Init warps.
            map.warps.init( 2 );

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

                        // Throw error if tiledata length is incorrect.
                        if ( tiledata.getLength() != map.width * map.height )
                        {
                            throw std::runtime_error( "Map “" + mapslug + "” tile layer poorly formed: size o’ tiles different from map height x map width." );
                        }

                        // Allocate tile layer.
                        TileLayer tilelayer;
                        tilelayer.scrollx = 1.0f;
                        tilelayer.scrolly = 1.0f;
                        tilelayer.offsetx = 0;
                        tilelayer.offsety = 0;
                        tilelayer.tiles = static_cast<uint_fast16_t *> ( calloc( tiledata.getLength(), sizeof( uint_fast16_t ) ) );

                        // Add each valid tile to tiles table.
                        tiledata.forEach
                        (
                            [ & ]( const JSONItem dataitem, uint_fast16_t i )
                            {
                                const uint_fast16_t tilen = dataitem.asInt();

                                // Throw error if we get non-tile value.
                                if ( tilen < 24577 && tilen > 0 )
                                {
                                    throw std::runtime_error( "Invalid tile # “" + std::to_string( tilen ) + "” used for tile layer on map “" + mapslug + "”." );
                                }

                                tilelayer.tiles[ i ] = tilen;
                            }
                        );

                        // Get parallax properties.
                        if ( layerobj.hasFloat( "parallaxx" ) )
                        {
                            tilelayer.scrollx = layerobj.getFloat( "parallaxx" );
                        }
                        if ( layerobj.hasFloat( "parallaxy" ) )
                        {
                            tilelayer.scrollx = layerobj.getFloat( "parallaxy" );
                        }

                        map.tiles.push( tilelayer );
                    }
                    else if ( typestring == "object" )
                    {
                        // Get list o’ tile indices.
                        const JSONArray tile_index_list = layerobj.getArray( "data" );

                        // Make sure tile index list length fills whole map.
                        if ( tile_index_list.getLength() != map.width * map.height )
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

                                map.objects[ i ] = tilen - 8192;
                            }
                        );
                    }
                    else if ( typestring == "collision" )
                    {
                        const JSONArray tiledata = layerobj.getArray( "data" );

                        // Throw error if tiledata length is incorrect.
                        if ( tiledata.getLength() != map.width * map.height )
                        {
                            throw std::runtime_error( "Map “" + mapslug + "” tile layer poorly formed: size o’ tiles different from map height x map width." );
                        }

                        // Add each non-0 tile to collision table.
                        tiledata.forEach
                        (
                            [ & ]( const JSONItem dataitem, uint_fast16_t i )
                            {
                                const uint_fast8_t tilen = dataitem.asInt();
                                if ( tilen > 0 )
                                {
                                    map.collision[ i ] = tilen;
                                }
                            }
                        );
                    }
                    else if ( typestring == "sprite" )
                    {
                        const JSONArray indices = layerobj.getArray( "data" );

                        // Throw error if tiledata length is incorrect.
                        if ( indices.getLength() != map.width * map.height )
                        {
                            throw std::runtime_error( "Map “" + mapslug + "” tile layer poorly formed: size o’ tiles different from map height x map width." );
                        }

                        // Add each non-0 sprite to sprites table.
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

                                map.sprites[ i ] = index - 16384;
                            }
                        );
                    }
                    else if ( typestring == "image" )
                    {
                        ImageLayer layer;
                        const std::string src = std::regex_replace( layerobj.getString( "image" ), std::regex( "../graphics/" ), "" );
                        layer.src = static_cast<char *> ( calloc( src.size() + 1, 1 ) );
                        strncpy( layer.src, src.c_str(), src.size() );
                        layer.repeatx = layerobj.getBoolOptional( "repeatx", false );
                        layer.repeaty = layerobj.getBoolOptional( "repeaty", false );
                        layer.scrollx = layerobj.getFloatOptional( "parallaxx", 1.0f );
                        layer.scrolly = layerobj.getFloatOptional( "parallaxy", 1.0f );
                        layer.offsetx = layerobj.getIntOptional( "offsetx", 0 );
                        layer.offsety = layerobj.getIntOptional( "offsety", 0 );
                        map.images.push( layer );
                    }
                    else if ( typestring == "warp" )
                    {
                        const JSONArray objects = layerobj.getArray( "objects" );
                        objects.forEach
                        (
                            [ & ] ( const JSONItem objectitem )
                            {
                                LevelWarp warp;
                                const JSONObject object = objectitem.asObject();

                                // Get warp left.
                                if ( object.hasFloat( "x" ) )
                                {
                                    warp.left = object.getFloat( "x" );
                                }
                                else if ( object.hasInt( "x" ) )
                                {
                                    warp.left = static_cast<float>( object.getInt( "x" ) );
                                }
                                else
                                {
                                    throw std::runtime_error( "Map “" + mapslug + "” warp missing x coordinate." );
                                }

                                // Get warp right.
                                if ( object.hasFloat( "width" ) )
                                {
                                    warp.right = warp.left + object.getFloat( "width" );
                                }
                                else if ( object.hasInt( "width" ) )
                                {
                                    warp.right = warp.left + static_cast<float>( object.getInt( "width" ) );
                                }
                                else
                                {
                                    throw std::runtime_error( "Map “" + mapslug + "” warp missing width." );
                                }

                                // Get warp top.
                                if ( object.hasFloat( "y" ) )
                                {
                                    warp.top = object.getFloat( "y" );
                                }
                                else if ( object.hasInt( "y" ) )
                                {
                                    warp.top = static_cast<float>( object.getInt( "y" ) );
                                }
                                else
                                {
                                    throw std::runtime_error( "Map “" + mapslug + "” warp missing y coordinate." );
                                }

                                // Get warp bottom.
                                if ( object.hasFloat( "height" ) )
                                {
                                    warp.bottom = warp.top + object.getFloat( "height" );
                                }
                                else if ( object.hasInt( "height" ) )
                                {
                                    warp.bottom = warp.top + static_cast<float>( object.getInt( "height" ) );
                                }
                                else
                                {
                                    throw std::runtime_error( "Map “" + mapslug + "” warp missing height." );
                                }

                                // Get other warp properties.
                                const JSONArray objectprops = object.getArray( "properties" );
                                objectprops.forEach
                                (
                                    [ & ] ( const JSONItem objpropitem )
                                    {
                                        const JSONObject objprop = objpropitem.asObject();
                                        const std::string name = objprop.getString( "name" );
                                        if ( name == "map" )
                                        {
                                            warp.map = static_cast<uint_fast8_t> ( objprop.getInt( "value" ) );
                                        }
                                        else if ( name == "warpx" )
                                        {
                                            warp.warpx = static_cast<uint_fast16_t> ( objprop.getInt( "value" ) );
                                        }
                                        else if ( name == "warpy" )
                                        {
                                            warp.warpy = static_cast<uint_fast16_t> ( objprop.getInt( "value" ) );
                                        }
                                    }
                                );

                                // Add warp to list.
                                map.warps.push( warp );
                            }
                        );
                    }
                }
            );
        }

        // Init level to use 1st map.
        current_map = 0;

        // Init sprites list.
        sprites.init( 16 );

        generateLevelData();

        // Init player.
        player.init();

        // Init inventory.
        inventory.init();

        // Make sure camera starts is right position.
        updateCamera();
    }

    void Level::destroy()
    {
        for ( uint_fast8_t i = 0; i < mapcount; ++i )
        {
            Map & map = maps[ i ];
            free( map.collision );
            free( map.sprites );
            free( map.objects );

            map.tiles.forEach
            (
                [&] ( auto & layer )
                {
                    free( layer.tiles );
                }
            );
            map.tiles.close();

            map.images.forEach
            (
                [&] ( auto & layer )
                {
                    free( layer.src );
                }
            );
            map.images.close();
            map.warps.close();
        }

        free( maps );
        free( objects );
        sprites.close();
    };

    void Level::update( float dt )
    {
        if ( BSL::Controls::heldMenu() )
        {
            BSL::Game::pushLevelPauseState();

            player.jump_lock = true; // Make sure player doesn’t jump after leaving menu.

            return; // If pausing, abort rest o’ update.
        }

        player.update( dt, *this );

        updateCamera();

        // Update sprites.
        for ( uint_fast16_t i = 0; i < sprites.count; ++i )
        {
            sprites.data[ i ].update( dt, *this );
        }

        // Update inventory.
        inventory.update( dt );
    };

    void Level::warp()
    {
        // Look for warp around player’s position.
        current_warp = nullptr;
        for ( uint_fast8_t i = 0; i < maps[ current_map ].warps.count; ++i )
        {
            const LevelWarp & warpitem = maps[ current_map ].warps.data[ i ];
            if
            (
                player.getCenterX() < warpitem.right
                && player.getCenterX() > warpitem.left
                && player.getCenterY() < warpitem.bottom
                && player.getCenterY() > warpitem.top
            )
            {
                current_warp = &warpitem;
                break;
            }
        }

        // If we couldn’t find a valid warp, skip warping.
        if ( !current_warp )
        {
            return;
        }

        // Set this level’s pointer as arg to fade-to state so it can access it.
        BSL::Game::FadeToArgs args;
        args.ptr = static_cast<void *> ( this );

        // Create fade-out event & run continueWarp function when it finishes.
        BSL::Game::fadeTo( &continueWarp, args );
    };

    void Level::finishWarp()
    {
        BSL::GFX::clearGraphics();

        current_map = current_warp->map;
        const Map & map = maps[ current_map ];

        // Clear sprites & objects.
        sprites.count = 0;
        free( objects );

        generateLevelData();

        // Update player.
        player.x = current_warp->warpx * BLOCK_SIZE;
        player.y = current_warp->warpy * BLOCK_SIZE;
        player.resetGFX();

        // Update inventory.
        inventory.resetGFX();

        // Make sure camera starts is right position.
        updateCamera();
    };

    void Level::updateCamera()
    {
        // Update camera.
        if ( player.getCenterX() > camera.x + CAMERA_RIGHT_EDGE )
        {
            camera.x = player.getCenterX() - CAMERA_RIGHT_EDGE;
            if ( camera.x > width * BLOCK_SIZE - WINDOW_WIDTH_PIXELS )
            {
                camera.x = width * BLOCK_SIZE - WINDOW_WIDTH_PIXELS;
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
            if ( camera.y > height * BLOCK_SIZE - WINDOW_HEIGHT_PIXELS )
            {
                camera.y = height * BLOCK_SIZE - WINDOW_HEIGHT_PIXELS;
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
    };

    void Level::generateLevelData()
    {
        const Map & map = maps[ current_map ];

        // Update general properties.
        width = map.width;
        height = map.height;
        collision = map.collision;

        const uint_fast16_t count = width * height;

        // Generate image GFX.
        map.images.forEach
        (
            [&] ( const ImageLayer & layer )
            {
                const uint_fast16_t texture = BSL::GFX::loadFileAsTexture( layer.src );
                BSL::GFX::addGraphicWallpaper
                (
                    texture,
                    {
                        { "repeatx", layer.repeatx },
                        { "repeaty", layer.repeaty },
                        { "scrollx", layer.scrollx },
                        { "scrolly", layer.scrolly },
                        { "offsetx", layer.offsetx },
                        { "offsety", layer.offsety }
                    }
                );
            }
        );

        // Generate tile GFX.
        map.tiles.forEach
        (
            [&] ( const TileLayer & layer )
            {
                BSL::GFX::Tile universal_tilemap[ count ];
                BSL::GFX::Tile specific_tilemap[ count ];

                // Zero out tilemaps so they don’t keep tiles from previous loops or have junk data.
                const size_t tilemapsize = count * sizeof( BSL::GFX::Tile );
                memset( universal_tilemap, 0x00, tilemapsize );
                memset( specific_tilemap, 0x00, tilemapsize );

                bool uses_universal = false;
                bool uses_tileset = false;
                for ( uint_fast16_t i = 0; i < count; ++i )
                {
                    const uint_fast16_t tilen = layer.tiles[ i ];

                    if ( tilen == 0 )
                    {
                        continue;
                    }

                    const bool notuniversal = tilen > 32768;
                    const uint_fast16_t realtilen = tilen - ( notuniversal ? 32769 : 24577 );

                    // Use correct tilemap based on index.
                    BSL::GFX::Tile * tilemap = notuniversal ? specific_tilemap : universal_tilemap;
                    tilemap[ i ].set = tilen > 24576;
                    tilemap[ i ].x = realtilen % 16;
                    tilemap[ i ].y = static_cast<uint_fast8_t> ( realtilen / 16.0 );
                    tilemap[ i ].animation = 0;
                    tilemap[ i ].frame = 0;

                    // Set flags if used.
                    if ( tilemap[ i ].set )
                    {
                        if ( notuniversal )
                        {
                            uses_tileset = true;
                        }
                        else
                        {
                            uses_universal = true;
                        }
                    }
                }

                // Only create universal tilemap if used.
                if ( uses_universal )
                {
                    const uint_fast16_t universal_texture = BSL::GFX::loadFileAsTexture( "tilesets/universal.png" );
                    BSL::GFX::addGraphicTilemap
                    (
                        universal_texture,
                        universal_tilemap,
                        width,
                        height,
                        {
                            { "scrollx", layer.scrollx },
                            { "scrolly", layer.scrolly },
                            { "offsetx", layer.offsetx },
                            { "offsety", layer.offsety },
                        }
                    );
                }

                // Only create tileset tilemap if used.
                if ( uses_tileset )
                {
                    const uint_fast16_t specific_texture = BSL::GFX::loadFileAsTexture( "tilesets/urban.png" );
                    BSL::GFX::addGraphicTilemap
                    (
                        specific_texture,
                        specific_tilemap,
                        width,
                        height,
                        {
                            { "scrollx", layer.scrollx },
                            { "scrolly", layer.scrolly },
                            { "offsetx", layer.offsetx },
                            { "offsety", layer.offsety },
                        }
                    );
                }
            }
        );

        // Init empty objects table & tilemap.
        objects = static_cast<Object *> ( calloc( width * height, sizeof( Object ) ) );
        BSL::GFX::Tile object_tilemap[ width * height ];
        memset( object_tilemap, 0, width * height * sizeof( BSL::GFX::Tile ) );

        // Go thru object data & generate objects & GFX.
        for ( uint_fast16_t i = 0; i < count; ++i )
        {
            const uint_fast16_t realtilen = map.objects[ i ];

            // Skip if it’s an empty tile.
            if ( realtilen == 0 )
            {
                continue;
            }

            const auto objtemp_container = ObjectFactory::get( realtilen - 1 );

            // Throw error if object template doesn’t exist.
            if ( !objtemp_container.has_value() )
            {
                throw std::runtime_error( "Invalid object #" + std::to_string( realtilen ) + " found in map." );
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

        // Create graphic layer for objects.
        const uint_fast16_t objects_texture = BSL::GFX::loadFileAsTexture( "tilesets/objects.png" );
        object_gfx = BSL::GFX::addGraphicTilemap
        (
            objects_texture,
            object_tilemap,
            width,
            height
        );

        // Init sprites.
        for ( uint_fast16_t i = 0; i < count; ++i )
        {
            // Skip empty indices.
            if ( map.sprites[ i ] == 0 )
            {
                continue;
            }

            const uint_fast16_t x = i % width;
            const uint_fast16_t y = std::floor( i / width );
            if ( map.sprites[ i ] == 1 )
            {
                player.x = x * BLOCK_SIZE;
                player.y = y * BLOCK_SIZE;
            }
            else
            {
                Sprite & sprite = sprites.pushEmptyGet();
                sprite.init( map.sprites[ i ], x, y );
            }
        }
    };

    static void continueWarp()
    {
        // Get level data from fade-to args.
        Level * level = static_cast<Level *> ( BSL::Game::getLastFadeToArgs().ptr );

        // Remove fade-to state.
        BSL::Game::popState();

        // Finalize map transition.
        level->finishWarp();

        // Add fade-in.
        BSL::Game::pushFadeIn();
    };
};