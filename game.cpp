#include "controls.hpp"
#include <cstring>
#include "game.hpp"
#include "gfx.hpp"
#include "json.hpp"
#include <vector>

namespace BSL::Game
{
    static constexpr unsigned int WATERTILEW = 8;
    static constexpr unsigned int WATERTILEH = 4;
    static constexpr unsigned int WATERW = WINDOW_WIDTH_PIXELS + WATERTILEW;
    static constexpr unsigned int WATERH = WATERTILEH;
    static constexpr unsigned int WATERROWS = ( WINDOW_HEIGHT_PIXELS - 7 * 8 ) / 4 + 3;
    static constexpr unsigned int BGGFXCOUNT = 4;
    static constexpr float CAMERA_RIGHT_EDGE = WINDOW_WIDTH_PIXELS * 0.667f;
    static constexpr float CAMERA_LEFT_EDGE = WINDOW_WIDTH_PIXELS * 0.333f;
    static constexpr float OWWINDOWH = 248.0f;
    static constexpr float CAMERA_BOTTOM_EDGE = OWWINDOWH * 0.667f;
    static constexpr float CAMERA_TOP_EDGE = OWWINDOWH * 0.333f;

    struct Warp
    {
        uint_fast8_t map;
        uint_fast8_t x;
        uint_fast8_t y;
    };

    enum class StateType
    {
        FADE_IN,
        FADE_TO_OW,
        TITLE,
        OVERWORLD,
        LEVEL
    };

    union GameStateData
    {
        struct
        {
            float speed;
            float opacity;
        }
        fade;
        struct
        {
            float speed;
            float opacity;
            Warp warp;
        }
        fadetoow;
        struct {
            BSL::GFX::RectGradient bg;
            BSL::GFX::Text title;
        }
        title;
        struct
        {
            BSL::GFX::RawSprite water[ WATERROWS ];
            float waterxodd;
            float waterxeven;
            struct
            {
                BSL::GFX::RawSprite gfx[ BGGFXCOUNT ];
                float x;
                float y;
            }
            bg;
            struct
            {
                BSL::GFX::Sprite gfx;
                float accx;
                float accy;
                float vx;
                float vy;
                float x;
                float y;

                constexpr inline float getCenterX()
                {
                    return x + 8.0f;
                }

                constexpr inline float getCenterY()
                {
                    return y + 8.0f;
                }

                constexpr inline float getLeftBoundary()
                {
                    return x + 5.0f;
                }

                constexpr inline float getRightBoundary()
                {
                    return x + 11.0f;
                }

                constexpr inline float getTopBoundary()
                {
                    return y + 4.0f;
                }

                constexpr inline float getBottomBoundary()
                {
                    return y + 12.0f;
                }
            }
            autumn;
            struct
            {
                float x;
                float y;
            }
            camera;
            struct
            {
                unsigned int w;
                unsigned int h;
                uint_fast8_t * collision;
                Warp * warps;
                uint_fast8_t warpcount;
            }
            map;
        }
        overworld;
        struct
        {
            BSL::GFX::Sprite sprite;
        }
        level;
    };

    struct GameState
    {
        StateType type;
        GameStateData data;
    };

    static GameState states_[ MAX_STATES ];
    static uint_fast8_t state_count;

    static void closeState( unsigned int n );
    static void fadeToOW( Warp warp );
    static void clearState();
    static void destroyState( uint_fast8_t staten );

    void init()
    {
        state_count = 2;
        BSL::GFX::setState( 0 );
        states_[ 0 ].type = StateType::TITLE;
        states_[ 0 ].data.title.bg = BSL::GFX::addGraphicRectGradient( 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS, 32, 200 );
        states_[ 0 ].data.title.title = BSL::GFX::addGraphicText
        (
            "Boskeopolis Land",
            {
                { "align", BSL::Align::CENTER },
                { "padding_top", 8u },
                { "shadow", true }
            }
        );
        BSL::GFX::setState( 1 );
        states_[ 1 ].type = StateType::FADE_IN;
        states_[ 1 ].data.fade.speed = 0.0f;
        states_[ 1 ].data.fade.opacity = 0.0f;
    };

    void update( float dt )
    {
        auto & current_state = states_[ state_count - 1 ];
        switch ( current_state.type )
        {
            case ( StateType::FADE_IN ):
            {
                auto & fade = current_state.data.fade;
                fade.speed += 0.001f * dt;
                if ( fade.speed > 0.05f )
                {
                    fade.speed = 0.05f;
                }
                fade.opacity += fade.speed * dt;

                if ( fade.opacity >= 1.0f )
                {
                    BSL::GFX::setCanvasOpacity( 1.0f );
                    popState();
                }
                else
                {
                    BSL::GFX::setCanvasOpacity( fade.opacity );
                }
            }
            break;
            case ( StateType::FADE_TO_OW ):
            {
                auto & fade = current_state.data.fadetoow;
                fade.speed += 0.001f * dt;
                if ( fade.speed > 0.05f )
                {
                    fade.speed = 0.05f;
                }
                fade.opacity -= fade.speed * dt;

                if ( fade.opacity <= 0.0f )
                {
                    fade.opacity = 0.0f;
                    BSL::GFX::setCanvasOpacity( fade.opacity );

                    // Save map.
                    const Warp inwarp = fade.warp;

                    clearState();

                    states_[ 0 ].type = StateType::OVERWORLD;
                    auto & data = states_[ 0 ].data.overworld;

                    // Generate O’erworld BG
                    static constexpr unsigned int OWBGTILEW = 16;
                    static constexpr unsigned int OWBGTILEH = 16;
                    static constexpr unsigned char OVERWORLD_BGTILE[ OWBGTILEW * OWBGTILEH ] =
                    {
                        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30,
                        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30,
                        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30,
                        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30,
                        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30,
                        0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
                        0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
                        0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,

                        0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
                        0x30, 0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
                        0x50, 0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
                        0x50, 0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50,
                        0x50, 0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50,
                        0x50, 0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50,
                        0x50, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50,
                        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x50, 0x50, 0x50, 0x50, 0x50
                    };

                    // BG top
                    static constexpr unsigned int OWBGTOPW = WINDOW_WIDTH_PIXELS + 16;
                    static constexpr unsigned int OWBGTOPH = OWBGTILEH + 1;
                    unsigned char top_pixels[ OWBGTOPW * OWBGTOPH ];
                    for ( unsigned int y = 0; y < OWBGTOPH; ++y )
                    {
                        for ( unsigned int x = 0; x < OWBGTOPW; x += OWBGTILEW )
                        {
                            memcpy( &top_pixels[ OWBGTOPW * y + x ], &OVERWORLD_BGTILE[ OWBGTILEW * ( y % OWBGTILEH ) ], OWBGTILEW );
                        }
                    }

                    data.bg.gfx[ 0 ] = BSL::GFX::addGraphicSpriteRaw
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
                    static constexpr unsigned int OWBGBOTTOMW = WINDOW_WIDTH_PIXELS + 16;
                    static constexpr unsigned int OWBGBOTTOMH = OWBGTILEH * 6;
                    unsigned char bottom_pixels[ OWBGBOTTOMW * OWBGBOTTOMH ];
                    for ( unsigned int y = 0; y < OWBGBOTTOMH; ++y )
                    {
                        for ( unsigned int x = 0; x < OWBGBOTTOMW; x += OWBGTILEW )
                        {
                            memcpy( &bottom_pixels[ OWBGBOTTOMW * y + x ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 6 ) % OWBGTILEH ) ], OWBGTILEW );
                        }
                    }

                    data.bg.gfx[ 1 ] = BSL::GFX::addGraphicSpriteRaw
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
                    static constexpr unsigned int OWBGSIDEW = OWBGTILEW + 3;
                    static constexpr unsigned int OWBGSIDEH = 260;
                    unsigned char left_pixels[ OWBGSIDEW * OWBGSIDEH ];
                    for ( unsigned int y = 0; y < OWBGSIDEH; ++y )
                    {
                        for ( unsigned int x = 0; x < OWBGSIDEW; x += OWBGTILEW )
                        {
                            // Don't write data beyond width o’ data.
                            const size_t size = std::min( OWBGSIDEW - x, OWBGTILEW );
                            memcpy( &left_pixels[ OWBGSIDEW * y + x ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 2 ) % OWBGTILEH ) ], size );
                        }
                    }

                    data.bg.gfx[ 2 ] = BSL::GFX::addGraphicSpriteRaw
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
                    for ( unsigned int y = 0; y < OWBGSIDEH; ++y )
                    {
                        // Offset by 13 pixels to keep aligned with other BG graphics.
                        memcpy( &right_pixels[ OWBGSIDEW * y ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 2 ) % OWBGTILEH ) + 13 ], 3 );
                        memcpy( &right_pixels[ OWBGSIDEW * y + 3 ], &OVERWORLD_BGTILE[ OWBGTILEW * ( ( y + 2 ) % OWBGTILEH ) ], OWBGTILEW );
                    }

                    data.bg.gfx[ 3 ] = BSL::GFX::addGraphicSpriteRaw
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
                        0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1,
                        0xA1, 0xA1, 0xA1, 0xFF, 0xFF, 0xA1, 0xA1, 0xA1,
                        0xA1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA1, 0xA1,
                        0xFF, 0xFF, 0xA1, 0xA1, 0xA1, 0xFF, 0xFF, 0xFF,
                    };
                    // Fill up single row o’ water tiles across screen width.
                    unsigned char water[ WATERW * WATERH ];
                    for ( unsigned int y = 0; y < WATERH; ++y )
                    {
                        for ( unsigned int x = 0; x < WATERW; x += WATERTILEW )
                        {
                            memcpy( &water[ WATERW * y + x ], &watertile[ WATERTILEW * y ], WATERTILEW );
                        }
                    }
                    // Generate graphics for each row o’ tiles.
                    for ( unsigned i = 0; i < WATERROWS; ++i )
                    {
                        data.water[ i ] = BSL::GFX::addGraphicSpriteRaw
                        (
                            water,
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
                    std::vector<Warp> tempwarps;
                    JSON json { "assets/maps/ow-" + std::to_string( inwarp.map ) + ".json" };
                    data.map.w = static_cast<unsigned int> ( json.getInt( "width" ) );
                    data.map.h = static_cast<unsigned int> ( json.getInt( "height" ) );
                    data.map.collision = static_cast<uint_fast8_t *> ( calloc( data.map.w * data.map.h, sizeof( uint_fast8_t ) ) );
                    BSL::GFX::Tile spritetiles[ data.map.w * data.map.h ];
                    for ( unsigned int i = 0; i < data.map.w * data.map.h; ++i )
                    {
                        memset( spritetiles, 0x00, data.map.w * data.map.h * sizeof( BSL::GFX::Tile ) );
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
                                            BSL::GFX::Tile tilemap[ data.map.w * data.map.h ];
                                            unsigned int i = 0;
                                            layerdata.forEach
                                            (
                                                [ & ]( const JSONItem dataitem )
                                                {
                                                    const unsigned int tilen = dataitem.asInt();
                                                    tilemap[ i ].set = tilen > 256;
                                                    tilemap[ i ].x = ( tilen - 257 ) % 16;
                                                    tilemap[ i ].y = static_cast<uint_fast8_t> ( ( tilen - 257 ) / 16.0 );
                                                    tilemap[ i ].animation = 0;
                                                    tilemap[ i ].frame = 0;
                                                    ++i;
                                                }
                                            );

                                            const unsigned int tilemap_texture = BSL::GFX::loadFileAsTexture( "tilesets/ow.png" );
                                            BSL::GFX::addGraphicTilemap
                                            (
                                                tilemap_texture,
                                                tilemap,
                                                data.map.w,
                                                data.map.h
                                            );
                                        }
                                        else if ( value == "collision" )
                                        {
                                            const JSONArray & layerdata = layer.getArray( "data" );
                                            unsigned int i = 0;
                                            layerdata.forEach
                                            (
                                                [ & ]( const JSONItem dataitem )
                                                {
                                                    const uint_fast8_t dataval = static_cast<uint_fast8_t> ( dataitem.asInt() );
                                                    if ( dataval > 0 )
                                                    {
                                                        data.map.collision[ i ] = dataval;
                                                    }
                                                    ++i;
                                                }
                                            );
                                        }
                                        else if ( value == "sprite" )
                                        {
                                            const JSONArray & layerdata = layer.getArray( "data" );
                                            unsigned int i = 0;
                                            layerdata.forEach
                                            (
                                                [ & ]( const JSONItem dataitem )
                                                {
                                                    const unsigned int dataval = static_cast<unsigned int> ( dataitem.asInt() );
                                                    switch ( dataval )
                                                    {
                                                        // Player
                                                        case ( 4353 ):
                                                        {
                                                            if ( inwarp.x == 0 && inwarp.y == 0 )
                                                            {
                                                                const unsigned int x = i % data.map.w;
                                                                const unsigned int y = static_cast<unsigned int>( i / data.map.w );
                                                                data.autumn.x = static_cast<float> ( x * 16 );
                                                                data.autumn.y = static_cast<float> ( y * 16 );
                                                            }
                                                        }
                                                        break;
                                                        default:
                                                        {
                                                            if ( dataval >= 4369 && dataval < 4369 + 256 )
                                                            {
                                                                const uint_fast8_t leveln = static_cast<uint_fast8_t> ( dataval - 4369 );
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
                                                    uint_fast16_t i = y * data.map.w + x;
                                                    data.map.collision[ i ] = static_cast<uint_fast8_t> ( tempwarps.size() + 16 );
                                                    Warp warp = { 0, 0, 0 };
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

                    data.map.warpcount = tempwarps.size();
                    const size_t warpssize = data.map.warpcount * sizeof( Warp );
                    data.map.warps = static_cast<Warp *> ( malloc( warpssize ) );
                    memcpy( data.map.warps, &tempwarps[ 0 ], warpssize );

                    const unsigned int sprite_texture = BSL::GFX::loadFileAsTexture( "sprites/ow.png" );
                    BSL::GFX::addGraphicTilemap
                    (
                        sprite_texture,
                        spritetiles,
                        data.map.w,
                        data.map.h
                    );

                    // Generate player sprite.
                    if ( inwarp.x != 0 || inwarp.y != 0 )
                    {
                        data.autumn.x = static_cast<float> ( inwarp.x * 16 );
                        data.autumn.y = static_cast<float> ( inwarp.y * 16 );
                    }
                    data.autumn.gfx = BSL::GFX::addGraphicSprite
                    (
                        sprite_texture,
                        static_cast<int> ( data.autumn.x ),
                        static_cast<int> ( data.autumn.y ),
                        16,
                        16
                    );

                    // Generate o’erworld windows.
                    BSL::GFX::addGraphicMenu( WINDOW_WIDTH_PIXELS, 40, 0, WINDOW_HEIGHT_PIXELS - 40 );
                    BSL::GFX::addGraphicMenu( WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS - 40, 0, 0, { { "bgcolor", 0x00 } } );

                    BSL::GFX::addGraphicText
                    (
                        "Wasabi Woods",
                        {
                            { "x", 8 },
                            { "y", 256 },
                            { "color", 0x01 }
                        }
                    );

                    // Generate fade-in state.
                    BSL::GFX::setState( 1 );
                    states_[ 1 ].type = StateType::FADE_IN;
                    states_[ 1 ].data.fade.speed = 0.0f;
                    states_[ 1 ].data.fade.opacity = 0.0f;
                    state_count = 2;
                }
                else
                {
                    BSL::GFX::setCanvasOpacity( fade.opacity );
                }
            }
            break;
            case ( StateType::OVERWORLD ):
            {
                auto & data = current_state.data.overworld;

                static constexpr float OVERWORLD_WATER_ACC = 1.0f / 8.0f;

                // Update BG animation.
                data.bg.x += OVERWORLD_WATER_ACC * dt;
                while ( data.bg.x > 16.0f )
                {
                    data.bg.x -= 16.0f;
                }
                data.bg.y += OVERWORLD_WATER_ACC * dt;
                while ( data.bg.y > 16.0f )
                {
                    data.bg.y -= 16.0f;
                }
                for ( unsigned int i = 0; i < BGGFXCOUNT; ++i )
                {
                    data.bg.gfx[ i ].setSrcX( data.bg.x );
                    data.bg.gfx[ i ].setSrcY( data.bg.y );
                }

                // Update water animation.
                data.waterxodd += OVERWORLD_WATER_ACC * dt;
                while ( data.waterxodd > 8.0f )
                {
                    data.waterxodd -= 8.0f;
                }
                data.waterxeven -= OVERWORLD_WATER_ACC * dt;
                while ( data.waterxeven < 0.0f )
                {
                    data.waterxeven += 8.0f;
                }
                const unsigned int camera_adjust_x = static_cast<unsigned int>( data.camera.x ) % 8;
                const unsigned int camera_adjust_y = static_cast<unsigned int>( data.camera.y ) % 8;
                const unsigned int waterxodd = ( static_cast<unsigned int> ( data.waterxodd ) + camera_adjust_x ) % 8;
                const unsigned int waterxeven = ( static_cast<unsigned int> ( data.waterxeven ) + camera_adjust_x ) % 8;
                for ( unsigned int i = 0; i < WATERROWS; i += 2 )
                {
                    data.water[ i ].setSrcX( waterxodd );
                    data.water[ i ].setY( WATERTILEH * i + camera_adjust_y );
                }
                for ( unsigned int i = 1; i < WATERROWS; i += 2 )
                {
                    data.water[ i ].setSrcX( waterxeven );
                    data.water[ i ].setY( WATERTILEH * i + camera_adjust_y );
                }

                // Update Autumn.
                static constexpr float OVERWORLD_AUTUMN_ACC = 0.25f;
                static constexpr float OVERWORLD_AUTUMN_MAX_SPEED = 2.0f;
                static constexpr float OVERWORLD_AUTUMN_HANDLING = 0.25f;

                // Autumn X movement.
                if ( BSL::Controls::heldRight() )
                {
                    data.autumn.accx = OVERWORLD_AUTUMN_ACC;
                }
                else if ( BSL::Controls::heldLeft() )
                {
                    data.autumn.accx = -OVERWORLD_AUTUMN_ACC;
                }
                else
                {
                    data.autumn.accx = 0.0f;
                }

                data.autumn.vx += data.autumn.accx * dt;

                if ( data.autumn.accx == 0.0f )
                {
                    data.autumn.vx /= ( 1.0f + OVERWORLD_AUTUMN_HANDLING * dt );
                }

                if ( data.autumn.vx > OVERWORLD_AUTUMN_MAX_SPEED )
                {
                    data.autumn.vx = OVERWORLD_AUTUMN_MAX_SPEED;
                }
                else if ( data.autumn.vx < -OVERWORLD_AUTUMN_MAX_SPEED )
                {
                    data.autumn.vx = -OVERWORLD_AUTUMN_MAX_SPEED;
                }



                // Autumn Y movement.
                if ( BSL::Controls::heldDown() )
                {
                    data.autumn.accy = OVERWORLD_AUTUMN_ACC;
                }
                else if ( BSL::Controls::heldUp() )
                {
                    data.autumn.accy = -OVERWORLD_AUTUMN_ACC;
                }
                else
                {
                    data.autumn.accy = 0.0f;
                }

                data.autumn.vy += data.autumn.accy * dt;

                if ( data.autumn.accy == 0.0f )
                {
                    data.autumn.vy /= ( 1.0f + OVERWORLD_AUTUMN_HANDLING * dt );
                }

                if ( data.autumn.vy > OVERWORLD_AUTUMN_MAX_SPEED )
                {
                    data.autumn.vy = OVERWORLD_AUTUMN_MAX_SPEED;
                }
                else if ( data.autumn.vy < -OVERWORLD_AUTUMN_MAX_SPEED )
                {
                    data.autumn.vy = -OVERWORLD_AUTUMN_MAX_SPEED;
                }

                // Collision
                if ( data.autumn.x + data.autumn.vx * dt < 0.0f )
                {
                    data.autumn.x = 0.0f;
                    data.autumn.vx = 0.0f;
                }
                else if ( data.autumn.x + BLOCK_SIZE + data.autumn.vx * dt >data.map.w * BLOCK_SIZE )
                {
                    data.autumn.x =data.map.w * BLOCK_SIZE - BLOCK_SIZE;
                    data.autumn.vx = 0.0f;
                }

                if ( data.autumn.y + data.autumn.vy * dt < 0.0f )
                {
                    data.autumn.y = 0.0f;
                    data.autumn.vy = 0.0f;
                }
                else if ( data.autumn.y + BLOCK_SIZE + data.autumn.vy * dt >data.map.h * BLOCK_SIZE )
                {
                    data.autumn.y =data.map.h * BLOCK_SIZE - BLOCK_SIZE;
                    data.autumn.vy = 0.0f;
                }

                const auto loopcount = std::max( static_cast<unsigned int> ( dt ), 1u );
                const float dti = dt / static_cast<float> ( loopcount );
                for ( unsigned int i = 0; i < loopcount; ++i )
                {
                    data.autumn.x += data.autumn.vx * dti;
                    data.autumn.y += data.autumn.vy * dti;

                    const unsigned int topy = static_cast<unsigned int> ( data.autumn.getTopBoundary() / 16.0 );
                    const unsigned int bottomy = static_cast<unsigned int> ( data.autumn.getBottomBoundary() / 16.0 );
                    const unsigned int yx = static_cast<unsigned int> ( ( data.autumn.x + 8.0 ) / 16.0 );
                    const unsigned int toplefti = topy *data.map.w + yx;
                    const unsigned int toprighti = topy *data.map.w + yx;
                    const unsigned int bottomlefti = bottomy *data.map.w + yx;
                    const unsigned int bottomrighti = bottomy *data.map.w + yx;
                    if ( data.map.collision[ toplefti ] == 0x01 || data.map.collision[ toprighti ] == 0x01 )
                    {
                        data.autumn.y += ( static_cast<float> ( topy + 1 ) * 16.0f ) - data.autumn.getTopBoundary();
                        data.autumn.vy *= -0.25f;
                    }
                    else if ( data.map.collision[ bottomlefti ] == 0x01 || data.map.collision[ bottomrighti ] == 0x01 )
                    {
                        data.autumn.y -= data.autumn.getBottomBoundary() - ( static_cast<float> ( bottomy ) * 16.0f );
                        data.autumn.vy *= -0.25f;
                    }
                    const unsigned int leftx = static_cast<unsigned int> ( data.autumn.getLeftBoundary() / 16.0 );
                    const unsigned int rightx = static_cast<unsigned int> ( data.autumn.getRightBoundary() / 16.0 );
                    const unsigned int topyx = static_cast<unsigned int> ( ( data.autumn.getTopBoundary() + 3.0 ) / 16.0 );
                    const unsigned int bottomyx = static_cast<unsigned int> ( ( data.autumn.getBottomBoundary() - 3.0 ) / 16.0 );
                    const unsigned int righttopi = topyx *data.map.w + rightx;
                    const unsigned int rightbottomi = bottomyx *data.map.w + rightx;
                    const unsigned int lefttopi = topyx *data.map.w + leftx;
                    const unsigned int leftbottomi = bottomyx *data.map.w + leftx;
                    if ( data.map.collision[ righttopi ] == 0x01 || data.map.collision[ rightbottomi ] == 0x01 )
                    {
                        data.autumn.x -= data.autumn.getRightBoundary() - ( static_cast<float> ( rightx ) * 16.0f );
                        data.autumn.vx *= -0.25f;
                    }
                    else if ( data.map.collision[ lefttopi ] == 0x01 || data.map.collision[ leftbottomi ] == 0x01 )
                    {
                        data.autumn.x += ( static_cast<float> ( leftx + 1 ) * 16.0f ) - data.autumn.getLeftBoundary();
                        data.autumn.vx *= -0.25f;
                    }
                }

                // Update final position.
                data.autumn.gfx.setX( data.autumn.x );
                data.autumn.gfx.setY( data.autumn.y );

                // Special collision.
                const unsigned int centerx = static_cast<unsigned int> ( data.autumn.getCenterX() / 16.0 );
                const unsigned int centery = static_cast<unsigned int> ( data.autumn.getCenterY() / 16.0 );
                const unsigned int centeri = centery * data.map.w + centerx;
                const uint_fast8_t centerval = data.map.collision[ centeri ];
                if ( centerval >= 16 && centerval < 32 )
                {
                    const uint_fast8_t warpn = centerval - 16;
                    fadeToOW( data.map.warps[ warpn ] );
                }

                // Autumn animation.
                data.autumn.gfx.setSrcX( ( static_cast<unsigned int> ( data.waterxodd ) % 2 ) * 16 );

                // Update camera.
                if ( data.autumn.getCenterX() > data.camera.x + CAMERA_RIGHT_EDGE )
                {
                    data.camera.x = data.autumn.getCenterX() - CAMERA_RIGHT_EDGE;
                    if ( data.camera.x > data.map.w * BLOCK_SIZE - WINDOW_WIDTH_PIXELS )
                    {
                        data.camera.x = data.map.w * BLOCK_SIZE - WINDOW_WIDTH_PIXELS;
                    }
                }
                else if ( data.autumn.getCenterX() < data.camera.x + CAMERA_LEFT_EDGE )
                {
                    data.camera.x = data.autumn.getCenterX() - CAMERA_LEFT_EDGE;
                    if ( data.camera.x < 0.0f )
                    {
                        data.camera.x = 0.0f;
                    }
                }
                if ( data.autumn.getCenterY() > data.camera.y + CAMERA_BOTTOM_EDGE )
                {
                    data.camera.y = data.autumn.getCenterY() - CAMERA_BOTTOM_EDGE;
                    if ( data.camera.y > data.map.h * BLOCK_SIZE - OWWINDOWH )
                    {
                        data.camera.y = data.map.h * BLOCK_SIZE - OWWINDOWH;
                    }
                }
                else if ( data.autumn.getCenterY() < data.camera.y + CAMERA_TOP_EDGE )
                {
                    data.camera.y = data.autumn.getCenterY() - CAMERA_TOP_EDGE;
                    if ( data.camera.y < 0.0f )
                    {
                        data.camera.y = 0.0f;
                    }
                }
                BSL::GFX::setCameraX( static_cast<unsigned int> ( data.camera.x ) );
                BSL::GFX::setCameraY( static_cast<unsigned int> ( data.camera.y ) );
            }
            break;
            case ( StateType::TITLE ):
            {
                if ( BSL::Controls::heldConfirm() )
                {
                    fadeToOW( { 0, 0, 0 } );
                }
            }
            break;
        }
    };

    void popState()
    {
        BSL::GFX::clearStateGraphics();
        destroyState( state_count - 1 );
        memset( &states_[ state_count - 1 ], 0, sizeof( GameState ) ); // Clear current state.
        BSL::GFX::setState( --state_count );
    };

    static void fadeToOW( Warp warp )
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::FADE_TO_OW;
        states_[ state_count ].data.fadetoow.speed = 0.0f;
        states_[ state_count ].data.fadetoow.opacity = 1.0f;
        states_[ state_count ].data.fadetoow.warp = warp;
        ++state_count;
    };

    static void clearState()
    {
        BSL::GFX::clearGraphics();
        BSL::GFX::setState( 0 );
        destroyState( 0 );
        memset( states_, 0, sizeof( GameState ) * MAX_STATES ); // Clear all states.
    };

    static void destroyState( uint_fast8_t staten )
    {
        switch ( states_[ staten ].type )
        {
            case ( StateType::OVERWORLD ):
            {
                free( states_[ staten ].data.overworld.map.collision );
                free( states_[ staten ].data.overworld.map.warps );
            }
            break;
        }
    };
}