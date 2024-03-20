#include "controls.hpp"
#include <cstring>
#include "game.hpp"
#include "gfx.hpp"
#include "level.hpp"
#include "level_pause.hpp"
#include "level_table.hpp"
#include "overworld.hpp"
#include "ow_level_menu.hpp"

namespace BSL::Game
{
    enum class StateType
    {
        FADE_IN,
        FADE_TO,
        TITLE,
        OVERWORLD,
        LEVEL,
        OW_LEVEL_OPEN,
        LV_MESSAGE,
        LV_PAUSE
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
            void ( * constructor )();
            FadeToArgs args;
        }
        fadeto;
        struct {
            BSL::GFX::RectGradient bg;
            BSL::GFX::Text title;
        }
        title;
        Overworld overworld;
        OWLevelMenu owlevelmenu;
        struct
        {
            bool confirmlock;
            uint_fast8_t levelid;
        }
        lvmessage;
        Level level;
        LevelPause levelpause;
    };

    struct GameState
    {
        StateType type;
        GameStateData data;
    };

    static GameState states_[ MAX_STATES ];
    static uint_fast8_t state_count;

    static void clearState();
    static void destroyState( uint_fast8_t staten );
    static void changeToOW();
    static void changeToLevel();

    void init()
    {
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
                current_state.data.overworld.update( dt );
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
                current_state.data.owlevelmenu.update( dt );
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
                current_state.data.level.update( dt );
            }
            break;
            case ( StateType::LV_PAUSE ):
            {
                current_state.data.levelpause.update( dt );
            }
            break;
        }
    };

    void popState()
    {
        BSL::GFX::clearStateGraphics();
        destroyState( state_count - 1 );
        memset( &states_[ state_count - 1 ], 0, sizeof( GameState ) ); // Clear current state.
        BSL::GFX::setState( --state_count - 1 );
    };

    void fadeTo( void ( * constructor )(), FadeToArgs args )
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::FADE_TO;
        states_[ state_count ].data.fadeto.speed = 0.0f;
        states_[ state_count ].data.fadeto.opacity = 1.0f;
        states_[ state_count ].data.fadeto.args = args;
        states_[ state_count ].data.fadeto.constructor = constructor;
        ++state_count;
    };

    void pushFadeIn()
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::FADE_IN;
        states_[ state_count ].data.fade.speed = 0.0f;
        states_[ state_count ].data.fade.opacity = 0.0f;
        ++state_count;
    };

    void pushLevelPauseState()
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::LV_PAUSE;
        states_[ state_count ].data.levelpause.init();
        ++state_count;
    };

    FadeToArgs getLastFadeToArgs()
    {
        return states_[ state_count - 1 ].data.fadeto.args;
    };

    void fadeToOW( OWWarp warp )
    {
        FadeToArgs args;
        args.ow.warp = warp;
        fadeTo( &changeToOW, args );
    };

    void pushLvMessageState()
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

    void pushOWLevelOpenMenu( uint_fast8_t levelid )
    {
        BSL::GFX::setState( state_count );
        states_[ state_count ].type = StateType::OW_LEVEL_OPEN;
        states_[ state_count ].data.owlevelmenu.init( levelid );
        ++state_count;
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
                states_[ staten ].data.overworld.destroy();
            }
            break;
            case ( StateType::LEVEL ):
            {
                states_[ staten ].data.level.destroy();
            }
            break;
        }
    };

    static void changeToOW()
    {
        // Save map from fadeto state.
        auto & args = states_[ state_count - 1 ].data.fadeto.args.ow;
        const OWWarp inwarp = args.warp;

        // Clear state.
        clearState();

        // Init OW state.
        states_[ 0 ].type = StateType::OVERWORLD;
        states_[ 0 ].data.overworld.init( inwarp );
        ++state_count;

        // Add fade in state.
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