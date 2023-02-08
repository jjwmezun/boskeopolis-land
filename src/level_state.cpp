#include "config.hpp"
#include "dir.hpp"
#include "game.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "nasringine/nasr.h"
#include "title_state.hpp"

namespace BSL
{
    LevelState::LevelState()
    :
        map_ ( "city-1" ),
        autumn_ ()
    {};
    void LevelState::update( Game & game, float dt )
    {
        autumn_.update( dt, input_, map_ );
        if ( Input::heldMenu() )
        {
            game.changeState( std::make_unique<TitleState>() );
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
    };
}