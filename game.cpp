#include "controls.hpp"
#include <cstring>
#include "game.hpp"
#include "gfx.hpp"

namespace BSL::Game
{
    enum class StateType
    {
        FADE_IN,
        FADE_TO_LEVEL,
        TITLE,
        LEVEL
    };

    struct FadeState
    {
        float speed;
        float opacity;
    };

    struct TitleState
    {
        BSL::GFX::RectGradient bg;
    };

    struct LevelState
    {
        BSL::GFX::Sprite sprite;
    };

    union GameStateData
    {
        FadeState fade;
        TitleState title;
        LevelState level;
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
        BSL::GFX::setState( 1 );
        states_[ 1 ].type = StateType::FADE_IN;
        states_[ 1 ].data.fade.speed = 0.0f;
        states_[ 1 ].data.fade.opacity = 0.0f;
    };

    void update( float dt )
    {
        switch ( states_[ state_count - 1 ].type )
        {
            case ( StateType::FADE_IN ):
            {
                auto & fade = states_[ state_count - 1 ].data.fade;
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
                auto & fade = states_[ state_count - 1 ].data.fade;
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
                    memset( &states_[ 0 ], 0, sizeof( GameState ) );
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