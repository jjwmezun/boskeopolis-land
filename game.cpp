#include "controls.hpp"
#include <cstring>
#include "game.hpp"
#include "gfx.hpp"
#include "json.hpp"
#include "level.hpp"
#include "level_table.hpp"
#include <stdexcept>
#include <vector>

namespace BSL::Game
{
    static constexpr unsigned int WATERTILEW = 8;
    static constexpr unsigned int WATERTILEH = 4;
    static constexpr unsigned int WATERW = WINDOW_WIDTH_PIXELS + WATERTILEW;
    static constexpr unsigned int WATERH = WATERTILEH;
    static constexpr unsigned int WATERROWS = ( WINDOW_HEIGHT_PIXELS - 7 * 8 ) / 4 + 3;
    static constexpr unsigned int BGGFXCOUNT = 4;
    static constexpr float OWWINDOWH = 248.0f;
    static constexpr float CAMERA_RIGHT_EDGE = WINDOW_WIDTH_PIXELS * 0.667f;
    static constexpr float CAMERA_LEFT_EDGE = WINDOW_WIDTH_PIXELS * 0.333f;
    static constexpr float CAMERA_BOTTOM_EDGE = OWWINDOWH * 0.667f;
    static constexpr float CAMERA_TOP_EDGE = OWWINDOWH * 0.333f;
    static constexpr unsigned int OWLEVELOPEN_OPTIONCOUNT = 2;
    static constexpr int OWLVOPEN_MENUX = static_cast<int> ( ( WINDOW_WIDTH_PIXELS - 160 ) / 2.0 );
    static constexpr int OWLVOPEN_MENUY = static_cast<int> ( ( WINDOW_HEIGHT_PIXELS - 32 ) / 2.0 );

    struct Warp
    {
        uint_fast8_t map;
        uint_fast8_t x;
        uint_fast8_t y;
    };

    enum class StateType
    {
        FADE_IN,
        FADE_TO,
        TITLE,
        OVERWORLD,
        LEVEL,
        OW_LEVEL_OPEN,
        LV_MESSAGE
    };

    union FadeToArgs
    {
        struct
        {
            Warp warp;
        }
        ow;
        struct
        {
            uint_fast8_t levelid;
        }
        level;
    }
    args;

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
            void ( * constructor )();
            FadeToArgs args;
        }
        fadeto;
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
                uint_fast8_t * levels;
                Warp * warps;
                uint_fast8_t warpcount;
            }
            map;
            struct
            {
                BSL::GFX::Text lvname;
                BSL::GFX::Counter gemscore;
                BSL::GFX::Timer timescore;
            }
            ui;
            uint_fast8_t prev_level;
            uint_fast8_t current_level;
        }
        overworld;
        struct
        {
            bool confirmlock;
            uint_fast8_t levelid;
            uint_fast8_t selection;
            struct
            {
                GFX::Rect selection;
                GFX::Text optiontext[ OWLEVELOPEN_OPTIONCOUNT ];
            }
            gfx;
        }
        owlevelopen;
        struct
        {
            bool confirmlock;
            uint_fast8_t levelid;
        }
        lvmessage;
        Level level;
    };

    struct GameState
    {
        StateType type;
        GameStateData data;
    };

    static GameState states_[ MAX_STATES ];
    static uint_fast8_t state_count;

    static void closeState( unsigned int n );
    static void fadeTo( void ( * constructor )(), FadeToArgs args );
    static void fadeToOW( Warp warp );
    static void clearState();
    static void destroyState( uint_fast8_t staten );
    static void pushOWLevelOpenMenu( uint_fast8_t levelid );
    static void updateOWAnimation( auto & data, float dt );
    static void updateOWLevelOpenGFX( auto & data, uint_fast8_t selection );
    static void pushLvMessageState();
    static void pushFadeIn();
    static void changeToOW();
    static void changeToLevel();

    void init()
    {
        BSL::loadLevelDataTable();
        BSL::GFX::setState( 0 );
        states_[ 0 ].type = StateType::TITLE;
        states_[ 0 ].data.title.bg = BSL::GFX::addGraphicRectGradient( 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS, 0, 255 );
        states_[ 0 ].data.title.title = BSL::GFX::addGraphicText
        (
            "Boskeopolis Land",
            {
                { "align", BSL::Align::CENTER },
                { "padding_top", 8u },
                { "shadow", true }
            }
        );
        state_count = 1;
        pushFadeIn();
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
            case ( StateType::FADE_TO ):
            {
                auto & fade = current_state.data.fadeto;
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
                    fade.constructor();
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

                // Warp collision.
                const unsigned int centerx = static_cast<unsigned int> ( data.autumn.getCenterX() / 16.0 );
                const unsigned int centery = static_cast<unsigned int> ( data.autumn.getCenterY() / 16.0 );
                const unsigned int centeri = centery * data.map.w + centerx;
                const uint_fast8_t centerval = data.map.collision[ centeri ];
                if ( centerval >= 16 && centerval < 32 )
                {
                    const uint_fast8_t warpn = centerval - 16;
                    fadeToOW( data.map.warps[ warpn ] );
                }

                // Level collision.
                const uint_fast8_t lvval = data.map.levels[ centeri ];

                // Only show if a valid level.
                if ( lvval > 0 && lvval <= level_table.size() )
                {
                    data.ui.lvname.setOpacity( 1.0f );
                    data.ui.gemscore.setOpacity( 1.0f );
                    data.ui.timescore.setOpacity( 1.0f );
                    const BSL::LevelData & lv = level_table[ lvval - 1 ];
                    data.current_level = lvval;

                    // Update level GFX if touching a different level.
                    if ( data.prev_level != data.current_level )
                    {
                        // Update level name.
                        data.ui.lvname.changeText
                        (
                            lv.name.c_str(),
                            {
                                { "x", 8 },
                                { "y", 256 },
                            }
                        );

                        // Update gem score.
                        data.ui.gemscore.changeNumber( lv.gemscore );

                        // Update time score.
                        data.ui.timescore.changeSeconds( lv.timescore );

                        data.prev_level = data.current_level;
                    }

                    // Open level menu on confirmation.
                    if ( BSL::Controls::heldConfirm() )
                    {
                        pushOWLevelOpenMenu( lvval - 1 );
                    }
                }
                // If not a valid level, hide.
                else
                {
                    data.current_level = 0;
                    data.ui.lvname.setOpacity( 0.0f );
                    data.ui.gemscore.setOpacity( 0.0f );
                    data.ui.timescore.setOpacity( 0.0f );
                }

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

                updateOWAnimation( data, dt );

                /*
                data.palette_animation += dt;
                while ( data.palette_animation >= 0.5f )
                {
                    data.palette_animation -= 0.5f;
                    ++data.current_pal;
                    if ( data.current_pal >= 360 )
                    {
                        data.current_pal = 0;
                    }
                    GFX::setPalette( &data.palettes[ data.current_pal ][ 0 ] );
                }*/
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
            case ( StateType::OW_LEVEL_OPEN ):
            {
                auto & prevstate = states_[ state_count - 2 ].data.overworld;
                auto & data = current_state.data.owlevelopen;
                updateOWAnimation( prevstate, dt );

                if ( !BSL::Controls::heldConfirm() )
                {
                    data.confirmlock = false;
                }

                if ( BSL::Controls::heldCancel() )
                {
                    updateOWLevelOpenGFX( data, 1 );
                    popState();
                }
                else if ( !data.confirmlock && BSL::Controls::heldConfirm() )
                {
                    FadeToArgs args;
                    args.level.levelid = data.levelid;
                    fadeTo( &pushLvMessageState, args );
                }
                else if ( BSL::Controls::heldDown() )
                {
                    if ( data.selection < OWLEVELOPEN_OPTIONCOUNT - 1 )
                    {
                        updateOWLevelOpenGFX( data, data.selection + 1 );
                    }
                }
                else if ( BSL::Controls::heldUp() )
                {
                    if ( data.selection > 0 )
                    {
                        updateOWLevelOpenGFX( data, data.selection - 1 );
                    }
                }
            }
            break;
            case ( StateType::LV_MESSAGE ):
            {
                auto & data = current_state.data.lvmessage;
                if ( !Controls::heldConfirm() )
                {
                    data.confirmlock = false;
                }
                if ( !data.confirmlock && Controls::heldConfirm() )
                {
                    FadeToArgs args;
                    args.level.levelid = data.levelid;
                    fadeTo( &changeToLevel, args );
                }
            }
            break;
            case ( StateType::LEVEL ):
            {
                auto & data = current_state.data.level;
                data.update( dt );
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

    static void fadeTo( void ( * constructor )(), FadeToArgs args )
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::FADE_TO;
        states_[ state_count ].data.fadeto.speed = 0.0f;
        states_[ state_count ].data.fadeto.opacity = 1.0f;
        states_[ state_count ].data.fadeto.args = args;
        states_[ state_count ].data.fadeto.constructor = constructor;
        ++state_count;
    };

    static void fadeToOW( Warp warp )
    {
        FadeToArgs args;
        args.ow.warp = warp;
        fadeTo( &changeToOW, args );
    };

    static void clearState()
    {
        BSL::GFX::clearGraphics();
        BSL::GFX::setState( 0 );
        for ( uint_fast8_t i = 0; i < state_count; ++i )
        {
            destroyState( i );
        }
        memset( states_, 0, sizeof( GameState ) * MAX_STATES ); // Clear all states.
        state_count = 0;
    };

    static void destroyState( uint_fast8_t staten )
    {
        switch ( states_[ staten ].type )
        {
            case ( StateType::OVERWORLD ):
            {
                free( states_[ staten ].data.overworld.map.collision );
                free( states_[ staten ].data.overworld.map.levels );
                free( states_[ staten ].data.overworld.map.warps );
            }
            break;
        }
    };

    static void pushOWLevelOpenMenu( uint_fast8_t levelid )
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::OW_LEVEL_OPEN;
        auto & data = states_[ state_count ].data.owlevelopen;
        data.levelid = levelid;
        data.selection = 0;
        BSL::GFX::addGraphicMenu
        (
            160,
            32,
            OWLVOPEN_MENUX,
            OWLVOPEN_MENUY
        );
        data.gfx.selection = BSL::GFX::addGraphicRect
        (
            OWLVOPEN_MENUX + 5,
            OWLVOPEN_MENUY + 5,
            149,
            11,
            1,
            {
                { "abs", true },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        data.gfx.optiontext[ 0 ] = BSL::GFX::addGraphicText
        (
            "Play Level",
            {
                { "x", OWLVOPEN_MENUX + 8 },
                { "y", OWLVOPEN_MENUY + 8 },
                { "color", 0xFF },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        data.gfx.optiontext[ 1 ] = BSL::GFX::addGraphicText
        (
            "Cancel",
            {
                { "x", OWLVOPEN_MENUX + 8 },
                { "y", OWLVOPEN_MENUY + 16 },
                { "color", 0x01 },
                { "layer", Layer::AFTER_FG_2 }
            }
        );
        data.confirmlock = true;
        ++state_count;
    };

    static void updateOWAnimation( auto & data, float dt )
    {
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

        // Autumn animation.
        data.autumn.gfx.setSrcX( ( static_cast<unsigned int> ( data.waterxodd ) % 2 ) * 16 );
    };

    static void updateOWLevelOpenGFX( auto & data, uint_fast8_t selection )
    {
        // Remove highlight from prev selection text.
        data.gfx.optiontext[ data.selection ].setColor( 0x01 );

        // Update selection.
        data.selection = selection;

        // Move selection bar bg.
        data.gfx.selection.setY( OWLVOPEN_MENUY + 5 + ( 11 * data.selection ) );

        // Set highlight for new selection text.
        data.gfx.optiontext[ data.selection ].setColor( 0xFF );
    };

    static void pushLvMessageState()
    {
        const uint_fast8_t levelid = states_[ state_count - 1 ].data.fadeto.args.level.levelid;

        clearState();
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::LV_MESSAGE;
        auto & data = states_[ state_count ].data.lvmessage;
        data.levelid = levelid;
        BSL::GFX::addGraphicRect
        (
            0,
            0,
            WINDOW_WIDTH_PIXELS,
            WINDOW_HEIGHT_PIXELS,
            192,
            {
                { "abs", true }
            }
        );
        BSL::GFX::addGraphicText
        (
            level_table[ levelid ].goal.message,
            {
                { "align", Align::CENTER },
                { "valign", Valign::MIDDLE },
                { "shadow", true }
            }
        );
        ++state_count;

        GFX::setPalette( "sunny" );

        pushFadeIn();
    };

    static void pushFadeIn()
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::FADE_IN;
        states_[ state_count ].data.fade.speed = 0.0f;
        states_[ state_count ].data.fade.opacity = 0.0f;
        ++state_count;
    };

    static void changeToOW()
    {
        auto & args = states_[ state_count - 1 ].data.fadeto.args.ow;

        // Save map.
        const Warp inwarp = args.warp;

        clearState();

        states_[ 0 ].type = StateType::OVERWORLD;
        auto & data = states_[ 0 ].data.overworld;

        // Generate O’erworld BG
        static constexpr unsigned int OWBGTILEW = 16;
        static constexpr unsigned int OWBGTILEH = 16;
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
            0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
            0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0,
            0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0,
            0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF,
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
        data.map.levels = static_cast<uint_fast8_t *> ( calloc( data.map.w * data.map.h, sizeof( uint_fast8_t ) ) );
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
                                                // Levels
                                                if ( dataval >= 4369 && dataval < 4369 + 255 )
                                                {
                                                    const uint_fast8_t leveln = static_cast<uint_fast8_t> ( dataval - 4368 );
                                                    data.map.levels[ i ] = leveln;
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

        // Generate UI level name.
        data.ui.lvname = BSL::GFX::addGraphicText
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
        data.ui.gemscore = BSL::GFX::addGraphicCounter
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
        data.ui.timescore = BSL::GFX::addGraphicTimer
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
                        data.palettes[ h ][ v ].r = v;
                        data.palettes[ h ][ v ].g = t;
                        data.palettes[ h ][ v ].b = p;
                    }
                    break;
                    case ( 1 ):
                    {
                        data.palettes[ h ][ v ].r = q;
                        data.palettes[ h ][ v ].g = v;
                        data.palettes[ h ][ v ].b = p;
                    }
                    break;
                    case ( 2 ):
                    {
                        data.palettes[ h ][ v ].r = p;
                        data.palettes[ h ][ v ].g = v;
                        data.palettes[ h ][ v ].b = t;
                    }
                    break;
                    case ( 3 ):
                    {
                        data.palettes[ h ][ v ].r = p;
                        data.palettes[ h ][ v ].g = q;
                        data.palettes[ h ][ v ].b = v;
                    }
                    break;
                    case ( 4 ):
                    {
                        data.palettes[ h ][ v ].r = t;
                        data.palettes[ h ][ v ].g = p;
                        data.palettes[ h ][ v ].b = v;
                    }
                    break;
                    default:
                    {
                        data.palettes[ h ][ v ].r = v;
                        data.palettes[ h ][ v ].g = p;
                        data.palettes[ h ][ v ].b = q;
                    }
                    break;
                }
            }
        }
        GFX::setPalette( &data.palettes[ data.current_pal ][ 0 ] );
        */
        GFX::setPalette( "owred" );

        ++state_count;

        pushFadeIn();
    };

    static void changeToLevel()
    {
        const uint_fast8_t levelid = states_[ state_count - 1 ].data.fadeto.args.level.levelid;

        clearState();
        states_[ 0 ].type = StateType::LEVEL;
        auto & data = states_[ 0 ].data.level;
        data.init( levelid );
        ++state_count;
        pushFadeIn();
    };
}