#include "config.hpp"
#include "dir.hpp"
#include "game.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "nasringine/nasr.h"
#include "pause_state.hpp"

namespace BSL
{
    LevelState::LevelState()
    :
        map_ ( "city-1" ),
        autumn_ (),
        clock_ ( 0 )
    {};

    void LevelState::update( Game & game, float dt )
    {
        autumn_.update( dt, input_, map_ );
        clock_.addSeconds( 1.0f / 60.0f * dt );
        if ( Input::pressedMenu() )
        {
            game.pushState( std::make_unique<PauseState>() );
        }
    };

    void LevelState::init( Game & game )
    {
        NasrSetGlobalPalette( 1 );
        NasrMoveCamera( 0, 0, BSL::WINDOW_WIDTH_PIXELS, BSL::WINDOW_HEIGHT_PIXELS );
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

        map_.init( game );
        autumn_.init( game );

        game.render().addMenuBox
        (
            Text::Align::LEFT,
            Text::Valign::TOP,
            static_cast<float>( WINDOW_WIDTH_PIXELS - 16 ),
            32.0f,
            {
                { "x", 8.0f },
                { "y", static_cast<float>( WINDOW_HEIGHT_PIXELS - 40 ) }
            }
        );

        clock_.init( game );
    };
}