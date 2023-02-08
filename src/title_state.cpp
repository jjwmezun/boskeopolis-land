#include "config.hpp"
#include "game.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "title_state.hpp"

namespace BSL
{
    void TitleState::update( Game & game, float dt )
    {
        if ( Input::heldJump() )
        {
            game.changeState( std::make_unique<LevelState>() );
        }
    };

    void TitleState::init( Game & game )
    {
        game.render().addRect
        (
            0,
            0,
            WINDOW_WIDTH_PIXELS,
            WINDOW_HEIGHT_PIXELS,
            255,
            true
        );
    };
}