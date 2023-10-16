#include "config.hpp"
#include "dir.hpp"
#include "game.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "nasringine/nasr.h"
#include "pause_state.hpp"

namespace BSL
{
    LevelState::LevelState() {};

    void LevelState::update( Game & game, float dt )
    {
        level_.update( game, input_, dt );
        if ( Input::pressedMenu() )
        {
            game.pushState( std::make_unique<PauseState>() );
        }
    };

    void LevelState::init( Game & game )
    {
        level_.init( game );
        update( game, 0.0f );
    };

    void LevelState::backToState( Game & game, int message )
    {
        if ( message == 1 )
        {
            level_.doWarp( game, input_ );
        }
    };
}