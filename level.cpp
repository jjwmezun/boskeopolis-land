#include "config.hpp"
#include "controls.hpp"
#include "gfx.hpp"
#include "json.hpp"
#include "level.hpp"
#include "level_table.hpp"
#include <regex>
#include <stdexcept>
#include <string>

namespace BSL
{
    static constexpr float CAMERA_RIGHT_EDGE = WINDOW_WIDTH_PIXELS * 0.667f;
    static constexpr float CAMERA_LEFT_EDGE = WINDOW_WIDTH_PIXELS * 0.333f;
    static constexpr float CAMERA_BOTTOM_EDGE = WINDOW_HEIGHT_PIXELS * 0.667f;
    static constexpr float CAMERA_TOP_EDGE = WINDOW_HEIGHT_PIXELS * 0.333f;
    static constexpr float START_SPEED = 0.15f;
    static constexpr float FALL_SPEED = 0.5f;
    static constexpr float GRAVITY = 6.0f;
    static constexpr float JUMP_ACC = 0.2f;
    static constexpr float JUMP_INIT = 2.0f;
    static constexpr float JUMP_MAX = 4.75f;
    static constexpr float autumn_walk_frames[ 4 ] = { 0.0f, 16.0f, 0.0f, 32.0f };

    template <typename A>
    struct Dict
    {
        void add( const std::string & key, A value )
        {
            values_.push_back( { key, value } );
        }

        A get( const std::string & key ) const
        {
            for ( auto & value : values_ )
            {
                if ( value.first == key )
                {
                    return value.second;
                }
            }
            throw std::runtime_error( "Could not find Dict entry for key “" + key + "”." );
        }

        std::vector<std::pair<std::string, A>> values_;
    };

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

                    if ( uses_universal )
                    {
                        const uint_fast16_t universal_texture = BSL::GFX::loadFileAsTexture( "tilesets/universal.png" );
                        BSL::GFX::addGraphicTilemap
                        (
                            universal_texture,
                            universal_tilemap,
                            map.w,
                            map.h
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
                            map.h
                        );
                    }
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

        uint_fast16_t player_texture = GFX::loadFileAsTexture( "sprites/autumn.png" );
        player.x = 50.0f;
        player.y = -26.0f;
        player.vx = 0.0f;
        player.gfx = GFX::addGraphicSprite
        (
            player_texture,
            200,
            static_cast<int_fast32_t> ( player.y ),
            16,
            26
        );
    }

    void Level::update( float dt )
    {
        // Check if running.
        const float start_speed = Controls::heldRun() ? START_SPEED * 2.0f : START_SPEED;
        const float max_speed = Controls::heldRun() ? MAX_SPEED * 2.0f : MAX_SPEED;

        // Autumn Y movement.
        // Falling & Jumping
        const bool going_fast = player.isGoingFast();
        const float gravity = BSL::Controls::heldJump() ? GRAVITY / 1.5f : GRAVITY;
        const float max_jump = going_fast ? JUMP_MAX * 1.1f : JUMP_MAX;
        const bool can_start_jump = player.jump_padding > 0.0f &&
            !player.jump_lock &&
            BSL::Controls::heldJump();

        // Continue jump.
        if ( player.is_jumping )
        {
            if ( BSL::Controls::heldJump() )
            {
                player.accy = -JUMP_ACC;
            }
            else
            {
                player.is_jumping = false;
                player.accy = 0.0f;
            }
        }
        // Start jump.
        else if ( can_start_jump )
        {
            player.is_jumping = true;
            player.vy = -JUMP_INIT;
            player.accy = -JUMP_ACC;
        }
        // Else, fall.
        else
        {
            player.accy = FALL_SPEED;
        }

        // Update player y speed.
        player.vy += player.accy * dt;
        if ( player.vy > gravity )
        {
            player.vy = gravity;
        }
        else if ( player.vy < -max_jump )
        {
            player.vy = -max_jump;
            player.is_jumping = false;
            player.accy = 0.0f;
        }

        // Reset on ground.
        player.on_ground = false;

        // Update jump padding.
        player.jump_padding = std::max( 0.0f, player.jump_padding - 1.0f * dt);

        // Update jump lock.
        player.jump_lock = Controls::heldJump();


        // Update Autumn.
        static constexpr float PLAYER_HANDLING = 0.25f;

        // Autumn X movement.
        if ( BSL::Controls::heldRight() )
        {
            player.accx = start_speed;
            player.gfx.setFlipX( true );
        }
        else if ( BSL::Controls::heldLeft() )
        {
            player.accx = -start_speed;
            player.gfx.setFlipX( false );
        }
        else
        {
            player.accx = 0.0f;
        }

        player.vx += player.accx * dt;

        if ( player.accx == 0.0f )
        {
            player.vx /= ( 1.0f + PLAYER_HANDLING * dt );
        }

        if ( player.vx > max_speed )
        {
            player.vx = max_speed;
        }
        else if ( player.vx < -max_speed )
        {
            player.vx = -max_speed;
        }


        // Collision
        if ( player.x + player.vx * dt < 0.0f )
        {
            player.x = 0.0f;
            player.vx = 0.0f;
        }
        else if ( player.x + BLOCK_SIZE + player.vx * dt > map.w * BLOCK_SIZE )
        {
            player.x = map.w * BLOCK_SIZE - BLOCK_SIZE;
            player.vx = 0.0f;
        }

        if ( player.y + player.vy * dt < 0.0f )
        {
            player.y = 0.0f;
            player.vy = 0.0f;
        }
        else if ( player.y + BLOCK_SIZE + player.vy * dt > map.h * BLOCK_SIZE )
        {
            player.y = map.h * BLOCK_SIZE - BLOCK_SIZE;
            player.vy = 0.0f;
        }

        const uint_fast8_t loopcount = std::max( static_cast<uint_fast8_t> ( dt ), static_cast<uint_fast8_t> ( 1 ) );
        const float dti = dt / static_cast<float> ( loopcount );
        for ( uint_fast8_t i = 0; i < loopcount; ++i )
        {
            player.x += player.vx * dti;
            player.y += player.vy * dti;

            const uint_fast32_t topy = static_cast<uint_fast32_t> ( player.getTopBoundary() / 16.0 );
            const uint_fast32_t bottomy = static_cast<uint_fast32_t> ( player.getBottomBoundary() / 16.0 );
            const uint_fast32_t yx = static_cast<uint_fast32_t> ( ( player.x + 8.0 ) / 16.0 );
            const uint_fast32_t yxl = static_cast<uint_fast32_t> ( ( player.x + 2.0 ) / 16.0 );
            const uint_fast32_t yxr = static_cast<uint_fast32_t> ( ( player.x + 14.0 ) / 16.0 );
            const uint_fast32_t toplefti = topy * map.w + yx;
            const uint_fast32_t toprighti = topy * map.w + yx;
            const uint_fast32_t bottomlefti = bottomy * map.w + yxl;
            const uint_fast32_t bottomrighti = bottomy * map.w + yxr;
            if ( collision[ toplefti ] == 0x01 || collision[ toprighti ] == 0x01 )
            {
                player.y += ( static_cast<float> ( topy + 1 ) * 16.0f ) - player.getTopBoundary();
                player.vy *= -0.25f;
                player.accy = 0.0f;
                player.is_jumping = false;
            }
            else if ( collision[ bottomlefti ] == 0x01 || collision[ bottomrighti ] == 0x01 )
            {
                player.y = ( static_cast<float> ( bottomy ) * 16.0f ) - 24.0f;
                player.vy = 0;
                player.accy = 0.0f;
                player.on_ground = true;
                player.jump_padding = player.isGoingFast() ? 8.0f : 2.0f;
            }
            const uint_fast32_t leftx = static_cast<uint_fast32_t> ( player.getLeftBoundary() / 16.0 );
            const uint_fast32_t rightx = static_cast<uint_fast32_t> ( player.getRightBoundary() / 16.0 );
            const uint_fast32_t topyx = static_cast<uint_fast32_t> ( ( player.getTopBoundary() + 3.0 ) / 16.0 );
            const uint_fast32_t bottomyx = static_cast<uint_fast32_t> ( ( player.getBottomBoundary() - 3.0 ) / 16.0 );
            const uint_fast32_t righttopi = topyx * map.w + rightx;
            const uint_fast32_t rightbottomi = bottomyx * map.w + rightx;
            const uint_fast32_t lefttopi = topyx * map.w + leftx;
            const uint_fast32_t leftbottomi = bottomyx * map.w + leftx;
            if ( collision[ righttopi ] == 0x01 || collision[ rightbottomi ] == 0x01 )
            {
                player.x -= player.getRightBoundary() - ( static_cast<float> ( rightx ) * 16.0f );
                player.vx *= -0.25f;
            }
            else if ( collision[ lefttopi ] == 0x01 || collision[ leftbottomi ] == 0x01 )
            {
                player.x += ( static_cast<float> ( leftx + 1 ) * 16.0f ) - player.getLeftBoundary();
                player.vx *= -0.25f;
            }
        }

        // Update final position.
        player.gfx.setX( player.x );
        player.gfx.setY( player.y );


        // Update animation.
        if ( !player.on_ground )
        {
            player.gfx.setSrcX( 48.0f );
            player.gfx.setSrcY( 0.0f );
        }
        else
        {
            // Update walking animation if not stopped.
            if ( std::abs( player.accx ) > 0.01f )
            {
                if ( ( player.walk_timer += dt ) >= 8.0f )
                {
                    player.walk_timer -= 8.0f;
                    ++player.walk_frame;
                    if ( player.walk_frame > 3 )
                    {
                        player.walk_frame = 0;
                    }
                }
                player.gfx.setSrcX( autumn_walk_frames[ player.walk_frame ] );
                player.gfx.setSrcY( 0.0f );
            }
            else
            {
                player.gfx.setSrcX( 0.0f );
                player.gfx.setSrcY( 0.0f );
            }
        }


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
    };
};