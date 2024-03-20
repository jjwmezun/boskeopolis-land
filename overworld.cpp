#include "controls.hpp"
#include <cstring>
#include "game.hpp"
#include "json.hpp"
#include "level_table.hpp"
#include "overworld.hpp"
#include <vector>

namespace BSL
{
    void Overworld::init( OWWarp inwarp )
    {
        // Generate O’erworld BG
        static constexpr uint_fast8_t OWBGTILEW = 16;
        static constexpr uint_fast8_t OWBGTILEH = 16;
        static constexpr unsigned char OVERWORLD_BGTILE[ OWBGTILEW * OWBGTILEH ] =
        {
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,

            0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80,
            0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80,
            0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80,
            0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80
        };

        // BG top
        static constexpr uint_fast16_t OWBGTOPW = WINDOW_WIDTH_PIXELS + 16;
        static constexpr uint_fast16_t OWBGTOPH = OWBGTILEH + 1;
        unsigned char top_pixels[ OWBGTOPW * OWBGTOPH ];
        for ( uint_fast16_t y = 0; y < OWBGTOPH; ++y )
        {
            for ( uint_fast16_t x = 0; x < OWBGTOPW; x += OWBGTILEW )
            {
                memcpy( &top_pixels[ OWBGTOPW * y + x ], &OVERWORLD_BGTILE[ OWBGTILEW * ( y % OWBGTILEH ) ], OWBGTILEW );
            }
        }

        bg.gfx[ 0 ] = BSL::GFX::addGraphicSpriteRaw
        (
            top_pixels,
            OWBGTOPW,
            OWBGTOPH,
            WINDOW_WIDTH_PIXELS,
            2,
            0,
            0
        );

        // BG bottom
        static constexpr uint_fast16_t OWBGBOTTOMW = WINDOW_WIDTH_PIXELS + 16;
        static constexpr uint_fast16_t OWBGBOTTOMH = OWBGTILEH * 6;
        unsigned char bottom_pixels[ OWBGBOTTOMW * OWBGBOTTOMH ];
        for ( uint_fast16_t y = 0; y < OWBGBOTTOMH; ++y )
        {
            for ( uint_fast16_t x = 0; x < OWBGBOTTOMW; x += OWBGTILEW )
            {
                memcpy( &bottom_pixels[ OWBGBOTTOMW * y + x ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 6 ) % OWBGTILEH ) ], OWBGTILEW );
            }
        }

        bg.gfx[ 1 ] = BSL::GFX::addGraphicSpriteRaw
        (
            bottom_pixels,
            OWBGBOTTOMW,
            OWBGBOTTOMH,
            WINDOW_WIDTH_PIXELS,
            42,
            0,
            WINDOW_HEIGHT_PIXELS - 40 - 2
        );

        // BG left
        static constexpr uint_fast16_t OWBGSIDEW = OWBGTILEW + 3;
        static constexpr uint_fast16_t OWBGSIDEH = 260;
        unsigned char left_pixels[ OWBGSIDEW * OWBGSIDEH ];
        for ( uint_fast16_t y = 0; y < OWBGSIDEH; ++y )
        {
            for ( uint_fast16_t x = 0; x < OWBGSIDEW; x += OWBGTILEW )
            {
                // Don't write data beyond width o’ 
                const size_t size = std::min( OWBGSIDEW - x, static_cast<uint_fast16_t>( OWBGTILEW ) );
                memcpy( &left_pixels[ OWBGSIDEW * y + x ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 2 ) % OWBGTILEH ) ], size );
            }
        }

        bg.gfx[ 2 ] = BSL::GFX::addGraphicSpriteRaw
        (
            left_pixels,
            OWBGSIDEW,
            OWBGSIDEH,
            3,
            244,
            0,
            2
        );

        // BG right
        unsigned char right_pixels[ OWBGSIDEW * OWBGSIDEH ];
        for ( uint_fast16_t y = 0; y < OWBGSIDEH; ++y )
        {
            // Offset by 13 pixels to keep aligned with other BG graphics.
            memcpy( &right_pixels[ OWBGSIDEW * y ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 2 ) % OWBGTILEH ) + 13 ], 3 );
            memcpy( &right_pixels[ OWBGSIDEW * y + 3 ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 2 ) % OWBGTILEH ) ], OWBGTILEW );
        }

        bg.gfx[ 3 ] = BSL::GFX::addGraphicSpriteRaw
        (
            right_pixels,
            OWBGSIDEW,
            OWBGSIDEH,
            3,
            244,
            WINDOW_WIDTH_PIXELS - 3,
            2
        );

        // Generate water
        static constexpr unsigned char watertile[ WATERTILEW * WATERTILEH ] =
        {
            0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
            0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0,
            0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0,
            0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF,
        };
        // Fill up single row o’ water tiles across screen width.
        unsigned char watergfx[ WATERW * WATERH ];
        for ( uint_fast16_t y = 0; y < WATERH; ++y )
        {
            for ( uint_fast16_t x = 0; x < WATERW; x += WATERTILEW )
            {
                memcpy( &watergfx[ WATERW * y + x ], &watertile[ WATERTILEW * y ], WATERTILEW );
            }
        }
        // Generate graphics for each row o’ tiles.
        for ( unsigned i = 0; i < WATERROWS; ++i )
        {
            water[ i ] = BSL::GFX::addGraphicSpriteRaw
            (
                watergfx,
                WATERW,
                WATERH,
                WINDOW_WIDTH_PIXELS - 14,
                WATERH,
                7,
                WATERTILEH * i + 7,
                { { "layer", BSL::Layer::BG_1 } }
            );
        }

        // Generate tilemap.
        std::vector<OWWarp> tempwarps;
        JSON json { "assets/maps/ow-" + std::to_string( inwarp.map ) + ".json" };
        map.w = static_cast<uint_fast16_t> ( json.getInt( "width" ) );
        map.h = static_cast<uint_fast16_t> ( json.getInt( "height" ) );
        map.collision = static_cast<uint_fast8_t *> ( calloc( map.w * map.h, sizeof( uint_fast8_t ) ) );
        map.levels = static_cast<uint_fast8_t *> ( calloc( map.w * map.h, sizeof( uint_fast8_t ) ) );
        BSL::GFX::Tile spritetiles[ map.w * map.h ];
        for ( uint_fast16_t i = 0; i < map.w * map.h; ++i )
        {
            memset( spritetiles, 0x00, map.w * map.h * sizeof( BSL::GFX::Tile ) );
        }

        JSONArray layers = json.getArray( "layers" );
        layers.forEach
        (
            [ & ]( const JSONItem layeritem )
            {
                const JSONObject layer = layeritem.asObject();
                const JSONArray props = layer.getArray( "properties" );
                props.forEach
                (
                    [ & ]( const JSONItem propitem )
                    {
                        const JSONObject propobj = propitem.asObject();
                        const std::string name = propobj.getString( "name" );
                        if ( name == "type" )
                        {
                            const std::string value = propobj.getString( "value" );
                            if ( value == "tile" )
                            {
                                const JSONArray & layerdata = layer.getArray( "data" );
                                BSL::GFX::Tile tilemap[ map.w * map.h ];
                                uint_fast16_t i = 0;
                                layerdata.forEach
                                (
                                    [ & ]( const JSONItem dataitem )
                                    {
                                        const uint_fast16_t tilen = dataitem.asInt();
                                        tilemap[ i ].set = tilen > 256;
                                        tilemap[ i ].x = ( tilen - 257 ) % 16;
                                        tilemap[ i ].y = static_cast<uint_fast8_t> ( ( tilen - 257 ) / 16.0 );
                                        tilemap[ i ].animation = 0;
                                        tilemap[ i ].frame = 0;
                                        ++i;
                                    }
                                );

                                const uint_fast16_t tilemap_texture = BSL::GFX::loadFileAsTexture( "tilesets/ow.png" );
                                BSL::GFX::addGraphicTilemap
                                (
                                    tilemap_texture,
                                    tilemap,
                                    map.w,
                                    map.h
                                );
                            }
                            else if ( value == "collision" )
                            {
                                const JSONArray & layerdata = layer.getArray( "data" );
                                uint_fast16_t i = 0;
                                layerdata.forEach
                                (
                                    [ & ]( const JSONItem dataitem )
                                    {
                                        const uint_fast8_t dataval = static_cast<uint_fast8_t> ( dataitem.asInt() );
                                        if ( dataval > 0 )
                                        {
                                            map.collision[ i ] = dataval;
                                        }
                                        ++i;
                                    }
                                );
                            }
                            else if ( value == "sprite" )
                            {
                                const JSONArray & layerdata = layer.getArray( "data" );
                                uint_fast16_t i = 0;
                                layerdata.forEach
                                (
                                    [ & ]( const JSONItem dataitem )
                                    {
                                        const uint_fast16_t dataval = static_cast<uint_fast16_t> ( dataitem.asInt() );
                                        switch ( dataval )
                                        {
                                            // Player
                                            case ( 4353 ):
                                            {
                                                if ( inwarp.x == 0 && inwarp.y == 0 )
                                                {
                                                    const uint_fast16_t x = i % map.w;
                                                    const uint_fast16_t y = static_cast<uint_fast16_t>( i / map.w );
                                                    autumn.x = static_cast<float> ( x * 16 );
                                                    autumn.y = static_cast<float> ( y * 16 );
                                                }
                                            }
                                            break;
                                            default:
                                            {
                                                // Levels
                                                if ( dataval >= 4369 && dataval < 4369 + 255 )
                                                {
                                                    const uint_fast8_t leveln = static_cast<uint_fast8_t> ( dataval - 4368 );
                                                    map.levels[ i ] = leveln;
                                                    spritetiles[ i ].set = true;
                                                    spritetiles[ i ].x = 2;
                                                    spritetiles[ i ].animation = 12;
                                                }
                                            }
                                            break;
                                        }
                                        ++i;
                                    }
                                );
                            }
                            else if ( value == "warp" )
                            {
                                const JSONArray objects = layer.getArray( "objects" );
                                objects.forEach
                                (
                                    [ & ]( const JSONItem objectitem )
                                    {
                                        const JSONObject obj = objectitem.asObject();
                                        const JSONArray props = obj.getArray( "properties" );
                                        uint_fast8_t x = 0;
                                        uint_fast8_t y = 0;
                                        if ( obj.hasInt( "x" ) )
                                        {
                                            x = static_cast<uint_fast8_t> ( obj.getInt( "x" ) / 16.0 );
                                        }
                                        else if ( obj.hasFloat( "x" ) )
                                        {
                                            x = static_cast<uint_fast8_t> ( obj.getFloat( "x" ) / 16.0 );
                                        }
                                        if ( obj.hasInt( "y" ) )
                                        {
                                            y = static_cast<uint_fast8_t> ( obj.getInt( "y" ) / 16.0 );
                                        }
                                        else if ( obj.hasFloat( "y" ) )
                                        {
                                            y = static_cast<uint_fast8_t> ( obj.getFloat( "y" ) / 16.0 );
                                        }
                                        uint_fast16_t i = y * map.w + x;
                                        map.collision[ i ] = static_cast<uint_fast8_t> ( tempwarps.size() + 16 );
                                        OWWarp warp = { 0, 0, 0 };
                                        props.forEach
                                        (
                                            [ & ]( const JSONItem propitem )
                                            {
                                                const JSONObject propobj = propitem.asObject();
                                                const std::string name = propobj.getString( "name" );
                                                if ( name == "map" )
                                                {
                                                    warp.map = static_cast<uint_fast8_t> ( propobj.getInt( "value" ) );
                                                }
                                                else if ( name == "x" )
                                                {
                                                    warp.x = static_cast<uint_fast8_t> ( propobj.getInt( "value" ) );
                                                }
                                                else if ( name == "y" )
                                                {
                                                    warp.y = static_cast<uint_fast8_t> ( propobj.getInt( "value" ) );
                                                }
                                            }
                                        );
                                        tempwarps.push_back( warp );
                                    }
                                );
                            }
                        }
                    }
                );
            }
        );

        map.warpcount = tempwarps.size();
        const size_t warpssize = map.warpcount * sizeof( OWWarp );
        map.warps = static_cast<OWWarp *> ( malloc( warpssize ) );
        memcpy( map.warps, &tempwarps[ 0 ], warpssize );

        const uint_fast16_t sprite_texture = BSL::GFX::loadFileAsTexture( "sprites/ow.png" );
        BSL::GFX::addGraphicTilemap
        (
            sprite_texture,
            spritetiles,
            map.w,
            map.h
        );

        // Generate player sprite.
        if ( inwarp.x != 0 || inwarp.y != 0 )
        {
            autumn.x = static_cast<float> ( inwarp.x * 16 );
            autumn.y = static_cast<float> ( inwarp.y * 16 );
        }
        autumn.gfx = BSL::GFX::addGraphicSprite
        (
            sprite_texture,
            static_cast<int> ( autumn.x ),
            static_cast<int> ( autumn.y ),
            16,
            16
        );

        // Make sure camera is set to correct position.
        updateCamera();

        // Generate o’erworld windows.
        BSL::GFX::addGraphicMenu( WINDOW_WIDTH_PIXELS, 40, 0, WINDOW_HEIGHT_PIXELS - 40 );
        BSL::GFX::addGraphicMenu( WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS - 40, 0, 0, { { "bgcolor", 0x00 } } );

        // Generate UI level name.
        ui.lvname = BSL::GFX::addGraphicText
        (
            "Missing Level",
            {
                { "x", 8 },
                { "y", 256 },
                { "color", 0x01 },
                { "opacity", 0.0f }
            }
        );

        // Add UI GFX for total ₧ counter.
        BSL::GFX::addGraphicCounter
        (
            8420,
            9,
            {
                { "x", 8 },
                { "y", 256 + 16 },
                { "color", 0x01 },
                { "prefix", "₧" }
            }
        );

        // Add UI GFX for level gem score.
        ui.gemscore = BSL::GFX::addGraphicCounter
        (
            0,
            5,
            {
                { "x", 456 },
                { "y", 256 },
                { "color", 0x01 },
                { "prefix", "₧" }
            }
        );

        // Add UI GFX for level time score.
        ui.timescore = BSL::GFX::addGraphicTimer
        (
            255,
            {
                { "x", 464 },
                { "y", 264 },
                { "color", 0x01 },
                { "prefix", "🕑" }
            }
        );

        // Init palette.
        /*
        const float s = 0.5f;
        for ( uint_fast16_t h = 0; h < 360; ++h )
        {
            const float hrel = h / 60.0;
            const uint_fast8_t type = static_cast<uint_fast8_t>( hrel );
            const float ff = hrel - type;
            for ( uint_fast16_t v = 0; v < 256; ++v )
            {
                const float p = v * (1.0 - s);
                const float q = v * (1.0 - (s * ff));
                const float t = v * (1.0 - (s * (1.0 - ff)));
                switch ( type )
                {
                    case ( 0 ):
                    {
                        palettes[ h ][ v ].r = v;
                        palettes[ h ][ v ].g = t;
                        palettes[ h ][ v ].b = p;
                    }
                    break;
                    case ( 1 ):
                    {
                        palettes[ h ][ v ].r = q;
                        palettes[ h ][ v ].g = v;
                        palettes[ h ][ v ].b = p;
                    }
                    break;
                    case ( 2 ):
                    {
                        palettes[ h ][ v ].r = p;
                        palettes[ h ][ v ].g = v;
                        palettes[ h ][ v ].b = t;
                    }
                    break;
                    case ( 3 ):
                    {
                        palettes[ h ][ v ].r = p;
                        palettes[ h ][ v ].g = q;
                        palettes[ h ][ v ].b = v;
                    }
                    break;
                    case ( 4 ):
                    {
                        palettes[ h ][ v ].r = t;
                        palettes[ h ][ v ].g = p;
                        palettes[ h ][ v ].b = v;
                    }
                    break;
                    default:
                    {
                        palettes[ h ][ v ].r = v;
                        palettes[ h ][ v ].g = p;
                        palettes[ h ][ v ].b = q;
                    }
                    break;
                }
            }
        }
        GFX::setPalette( &palettes[ current_pal ][ 0 ] );
        */

        // Init palette.
        GFX::setPalette( "owred" );

        // Init lock.
        levelselectlock = true;
    };

    void Overworld::destroy()
    {
        free( map.collision );
        free( map.levels );
        free( map.warps );
    };

    void Overworld::update( float dt )
    {
        // Update Autumn.
        static constexpr float OVERWORLD_AUTUMN_ACC = 0.25f;
        static constexpr float OVERWORLD_AUTUMN_MAX_SPEED = 2.0f;
        static constexpr float OVERWORLD_AUTUMN_HANDLING = 0.25f;


        // Autumn X movement.
        if ( BSL::Controls::heldRight() )
        {
            autumn.accx = OVERWORLD_AUTUMN_ACC;
        }
        else if ( BSL::Controls::heldLeft() )
        {
            autumn.accx = -OVERWORLD_AUTUMN_ACC;
        }
        else
        {
            autumn.accx = 0.0f;
        }

        autumn.vx += autumn.accx * dt;

        if ( autumn.accx == 0.0f )
        {
            autumn.vx /= ( 1.0f + OVERWORLD_AUTUMN_HANDLING * dt );
        }

        if ( autumn.vx > OVERWORLD_AUTUMN_MAX_SPEED )
        {
            autumn.vx = OVERWORLD_AUTUMN_MAX_SPEED;
        }
        else if ( autumn.vx < -OVERWORLD_AUTUMN_MAX_SPEED )
        {
            autumn.vx = -OVERWORLD_AUTUMN_MAX_SPEED;
        }


        // Autumn Y movement.
        if ( BSL::Controls::heldDown() )
        {
            autumn.accy = OVERWORLD_AUTUMN_ACC;
        }
        else if ( BSL::Controls::heldUp() )
        {
            autumn.accy = -OVERWORLD_AUTUMN_ACC;
        }
        else
        {
            autumn.accy = 0.0f;
        }

        autumn.vy += autumn.accy * dt;

        if ( autumn.accy == 0.0f )
        {
            autumn.vy /= ( 1.0f + OVERWORLD_AUTUMN_HANDLING * dt );
        }

        if ( autumn.vy > OVERWORLD_AUTUMN_MAX_SPEED )
        {
            autumn.vy = OVERWORLD_AUTUMN_MAX_SPEED;
        }
        else if ( autumn.vy < -OVERWORLD_AUTUMN_MAX_SPEED )
        {
            autumn.vy = -OVERWORLD_AUTUMN_MAX_SPEED;
        }

        // Collision
        if ( autumn.x + autumn.vx * dt < 0.0f )
        {
            autumn.x = 0.0f;
            autumn.vx = 0.0f;
        }
        else if ( autumn.x + BLOCK_SIZE + autumn.vx * dt >map.w * BLOCK_SIZE )
        {
            autumn.x =map.w * BLOCK_SIZE - BLOCK_SIZE;
            autumn.vx = 0.0f;
        }

        if ( autumn.y + autumn.vy * dt < 0.0f )
        {
            autumn.y = 0.0f;
            autumn.vy = 0.0f;
        }
        else if ( autumn.y + BLOCK_SIZE + autumn.vy * dt >map.h * BLOCK_SIZE )
        {
            autumn.y =map.h * BLOCK_SIZE - BLOCK_SIZE;
            autumn.vy = 0.0f;
        }

        const auto loopcount = std::max( static_cast<uint_fast16_t> ( dt ), static_cast<uint_fast16_t> ( 1 ) );
        const float dti = dt / static_cast<float> ( loopcount );
        for ( uint_fast16_t i = 0; i < loopcount; ++i )
        {
            autumn.x += autumn.vx * dti;
            autumn.y += autumn.vy * dti;

            const uint_fast16_t topy = static_cast<uint_fast16_t> ( autumn.getTopBoundary() / 16.0 );
            const uint_fast16_t bottomy = static_cast<uint_fast16_t> ( autumn.getBottomBoundary() / 16.0 );
            const uint_fast16_t yx = static_cast<uint_fast16_t> ( ( autumn.x + 8.0 ) / 16.0 );
            const uint_fast16_t toplefti = topy *map.w + yx;
            const uint_fast16_t toprighti = topy *map.w + yx;
            const uint_fast16_t bottomlefti = bottomy *map.w + yx;
            const uint_fast16_t bottomrighti = bottomy *map.w + yx;
            if ( map.collision[ toplefti ] == 0x01 || map.collision[ toprighti ] == 0x01 )
            {
                autumn.y += ( static_cast<float> ( topy + 1 ) * 16.0f ) - autumn.getTopBoundary();
                autumn.vy *= -0.25f;
            }
            else if ( map.collision[ bottomlefti ] == 0x01 || map.collision[ bottomrighti ] == 0x01 )
            {
                autumn.y -= autumn.getBottomBoundary() - ( static_cast<float> ( bottomy ) * 16.0f );
                autumn.vy *= -0.25f;
            }
            const uint_fast16_t leftx = static_cast<uint_fast16_t> ( autumn.getLeftBoundary() / 16.0 );
            const uint_fast16_t rightx = static_cast<uint_fast16_t> ( autumn.getRightBoundary() / 16.0 );
            const uint_fast16_t topyx = static_cast<uint_fast16_t> ( ( autumn.getTopBoundary() + 3.0 ) / 16.0 );
            const uint_fast16_t bottomyx = static_cast<uint_fast16_t> ( ( autumn.getBottomBoundary() - 3.0 ) / 16.0 );
            const uint_fast16_t righttopi = topyx *map.w + rightx;
            const uint_fast16_t rightbottomi = bottomyx *map.w + rightx;
            const uint_fast16_t lefttopi = topyx *map.w + leftx;
            const uint_fast16_t leftbottomi = bottomyx *map.w + leftx;
            if ( map.collision[ righttopi ] == 0x01 || map.collision[ rightbottomi ] == 0x01 )
            {
                autumn.x -= autumn.getRightBoundary() - ( static_cast<float> ( rightx ) * 16.0f );
                autumn.vx *= -0.25f;
            }
            else if ( map.collision[ lefttopi ] == 0x01 || map.collision[ leftbottomi ] == 0x01 )
            {
                autumn.x += ( static_cast<float> ( leftx + 1 ) * 16.0f ) - autumn.getLeftBoundary();
                autumn.vx *= -0.25f;
            }
        }

        // Update final position.
        autumn.gfx.setX( autumn.x );
        autumn.gfx.setY( autumn.y );

        // Warp collision.
        const uint_fast16_t centerx = static_cast<uint_fast16_t> ( autumn.getCenterX() / 16.0 );
        const uint_fast16_t centery = static_cast<uint_fast16_t> ( autumn.getCenterY() / 16.0 );
        const uint_fast16_t centeri = centery * map.w + centerx;
        const uint_fast8_t centerval = map.collision[ centeri ];
        if ( centerval >= 16 && centerval < 32 )
        {
            const uint_fast8_t warpn = centerval - 16;
            BSL::Game::fadeToOW( map.warps[ warpn ] );
        }

        // Level collision.
        const uint_fast8_t lvval = map.levels[ centeri ];

        // Only show if a valid level.
        if ( lvval > 0 && lvval <= level_table.size() )
        {
            ui.lvname.setOpacity( 1.0f );
            ui.gemscore.setOpacity( 1.0f );
            ui.timescore.setOpacity( 1.0f );
            const BSL::LevelData & lv = level_table[ lvval - 1 ];
            current_level = lvval;

            // Update level GFX if touching a different level.
            if ( prev_level != current_level )
            {
                // Update level name.
                ui.lvname.changeText
                (
                    lv.name.c_str(),
                    {
                        { "x", 8 },
                        { "y", 256 },
                    }
                );

                // Update gem score.
                ui.gemscore.changeNumber( lv.gemscore );

                // Update time score.
                ui.timescore.changeSeconds( lv.timescore );

                prev_level = current_level;
            }

            // Open level menu on confirmation.
            if ( !levelselectlock && BSL::Controls::heldConfirm() )
            {
                BSL::Game::pushOWLevelOpenMenu( lvval - 1 );
            }
        }
        // If not a valid level, hide.
        else
        {
            current_level = 0;
            ui.lvname.setOpacity( 0.0f );
            ui.gemscore.setOpacity( 0.0f );
            ui.timescore.setOpacity( 0.0f );
        }

        // Update level select lock.
        levelselectlock = BSL::Controls::heldConfirm();

        updateCamera();
        updateAnimation( dt );

        /*
        palette_animation += dt;
        while ( palette_animation >= 0.5f )
        {
            palette_animation -= 0.5f;
            ++current_pal;
            if ( current_pal >= 360 )
            {
                current_pal = 0;
            }
            GFX::setPalette( &palettes[ current_pal ][ 0 ] );
        }*/
    };

    void Overworld::updateCamera()
    {
        if ( autumn.getCenterX() > camera.x + CAMERA_RIGHT_EDGE )
        {
            camera.x = autumn.getCenterX() - CAMERA_RIGHT_EDGE;
            if ( camera.x > map.w * BLOCK_SIZE - WINDOW_WIDTH_PIXELS )
            {
                camera.x = map.w * BLOCK_SIZE - WINDOW_WIDTH_PIXELS;
            }
        }
        else if ( autumn.getCenterX() < camera.x + CAMERA_LEFT_EDGE )
        {
            camera.x = autumn.getCenterX() - CAMERA_LEFT_EDGE;
            if ( camera.x < 0.0f )
            {
                camera.x = 0.0f;
            }
        }
        if ( autumn.getCenterY() > camera.y + CAMERA_BOTTOM_EDGE )
        {
            camera.y = autumn.getCenterY() - CAMERA_BOTTOM_EDGE;
            if ( camera.y > map.h * BLOCK_SIZE - OWWINDOWH )
            {
                camera.y = map.h * BLOCK_SIZE - OWWINDOWH;
            }
        }
        else if ( autumn.getCenterY() < camera.y + CAMERA_TOP_EDGE )
        {
            camera.y = autumn.getCenterY() - CAMERA_TOP_EDGE;
            if ( camera.y < 0.0f )
            {
                camera.y = 0.0f;
            }
        }
        BSL::GFX::setCameraX( static_cast<uint_fast16_t> ( camera.x ) );
        BSL::GFX::setCameraY( static_cast<uint_fast16_t> ( camera.y ) );
    };

    void Overworld::updateAnimation( float dt )
    {
        static constexpr float OVERWORLD_WATER_ACC = 1.0f / 8.0f;

        // Update BG animation.
        bg.x += OVERWORLD_WATER_ACC * dt;
        while ( bg.x > 16.0f )
        {
            bg.x -= 16.0f;
        }
        bg.y += OVERWORLD_WATER_ACC * dt;
        while ( bg.y > 16.0f )
        {
            bg.y -= 16.0f;
        }
        for ( uint_fast8_t i = 0; i < BGGFXCOUNT; ++i )
        {
            bg.gfx[ i ].setSrcX( bg.x );
            bg.gfx[ i ].setSrcY( bg.y );
        }

        // Update water animation.
        waterxodd += OVERWORLD_WATER_ACC * dt;
        while ( waterxodd > 8.0f )
        {
            waterxodd -= 8.0f;
        }
        waterxeven -= OVERWORLD_WATER_ACC * dt;
        while ( waterxeven < 0.0f )
        {
            waterxeven += 8.0f;
        }
        const uint_fast16_t camera_adjust_x = static_cast<uint_fast16_t>( camera.x ) % 8;
        const uint_fast16_t camera_adjust_y = static_cast<uint_fast16_t>( camera.y ) % 8;
        const uint_fast16_t relwaterxodd = ( static_cast<uint_fast16_t> ( waterxodd ) + camera_adjust_x ) % 8;
        const uint_fast16_t relwaterxeven = ( static_cast<uint_fast16_t> ( waterxeven ) + camera_adjust_x ) % 8;
        for ( uint_fast16_t i = 0; i < WATERROWS; i += 2 )
        {
            water[ i ].setSrcX( relwaterxodd );
            water[ i ].setY( WATERTILEH * i + camera_adjust_y );
        }
        for ( uint_fast16_t i = 1; i < WATERROWS; i += 2 )
        {
            water[ i ].setSrcX( relwaterxeven );
            water[ i ].setY( WATERTILEH * i + camera_adjust_y );
        }

        // Autumn animation.
        autumn.gfx.setSrcX( ( static_cast<uint_fast16_t> ( relwaterxodd ) % 2 ) * 16 );
    };
}