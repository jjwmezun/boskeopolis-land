#include "controls.hpp"
#include <cstring>
#include "game.hpp"
#include "gfx.hpp"

namespace BSL::Game
{
    static constexpr unsigned int WATERTILEW = 8;
    static constexpr unsigned int WATERTILEH = 4;
    static constexpr unsigned int WATERW = WINDOW_WIDTH_PIXELS + WATERTILEW;
    static constexpr unsigned int WATERH = WATERTILEH;
    static constexpr unsigned int WATERROWS = ( WINDOW_HEIGHT_PIXELS - 7 * 8 ) / 4 + 1;
    static constexpr unsigned int BGGFXCOUNT = 4;

    enum class StateType
    {
        FADE_IN,
        FADE_TO_LEVEL,
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
            }
            autumn;
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
                    fade.opacity = 1.0f;
                    popState();
                }
                BSL::GFX::setCanvasOpacity( fade.opacity );
            }
            break;
            case ( StateType::FADE_TO_LEVEL ):
            {
                auto & fade = current_state.data.fade;
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
                    BSL::GFX::clearGraphics();
                    BSL::GFX::setState( 0 );
                    memset( &states_[ 0 ], 0, sizeof( GameState ) ); // Clear state.
                    states_[ 0 ].type = StateType::OVERWORLD;
                    unsigned int autumn_texture = BSL::GFX::loadFileAsTexture( "sprites/autumn-overworld.png" );
                    states_[ 0 ].data.overworld.autumn.gfx = BSL::GFX::addGraphicSprite
                    (
                        autumn_texture,
                        0,
                        0,
                        16,
                        16
                    );

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

                    states_[ 0 ].data.overworld.bg.gfx[ 0 ] = BSL::GFX::addGraphicSpriteRaw
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

                    states_[ 0 ].data.overworld.bg.gfx[ 1 ] = BSL::GFX::addGraphicSpriteRaw
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

                    states_[ 0 ].data.overworld.bg.gfx[ 2 ] = BSL::GFX::addGraphicSpriteRaw
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

                    states_[ 0 ].data.overworld.bg.gfx[ 3 ] = BSL::GFX::addGraphicSpriteRaw
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
                        states_[ 0 ].data.overworld.water[ i ] = BSL::GFX::addGraphicSpriteRaw
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

                    /*
                    states_[ 0 ].type = StateType::LEVEL;
                    unsigned int t = BSL::GFX::loadFileAsTexture( "sprites/autumn.png" );
                    states_[ 0 ].data.level.sprite = BSL::GFX::addGraphicSprite
                    (
                        t,
                        0,
                        0,
                        16,
                        26
                    );
                    */
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
                for ( unsigned int i = 0; i < WATERROWS; i += 2 )
                {
                    data.water[ i ].setSrcX( data.waterxodd );
                }
                for ( unsigned int i = 1; i < WATERROWS; i += 2 )
                {
                    data.water[ i ].setSrcX( data.waterxeven );
                }

                // Update Autumn.
                static constexpr float OVERWORLD_AUTUMN_ACC = 0.25f;
                static constexpr float OVERWORLD_AUTUMN_MAX_SPEED = 4.0f;
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

                data.autumn.x += data.autumn.vx * dt;

                data.autumn.gfx.setX( data.autumn.x );

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

                data.autumn.y += data.autumn.vy * dt;

                data.autumn.gfx.setY( data.autumn.y );

                // Autumn animation.
                data.autumn.gfx.setSrcX( ( static_cast<unsigned int> ( data.waterxodd ) % 2 ) * 16 );
            }
            break;
            case ( StateType::TITLE ):
            {
                if ( BSL::Controls::heldConfirm() )
                {
                    BSL::GFX::setState( state_count );
                    states_[ state_count ].type = StateType::FADE_TO_LEVEL;
                    states_[ state_count ].data.fade.speed = 0.0f;
                    states_[ state_count ].data.fade.opacity = 1.0f;
                    ++state_count;
                }
            }
            break;
        }
    };

    void popState()
    {
        BSL::GFX::setState( --state_count );
    };
}