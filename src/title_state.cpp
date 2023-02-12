#include "config.hpp"
#include "game.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "title_state.hpp"

namespace BSL
{
    void TitleState::update( Game & game, float dt )
    {
        if ( Input::pressedJump() )
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

        game.render().addTextGradient
        (
            "Boskeopolis Land",
            Dir::XY::DOWN,
            224,
            64,
            0.0f,
            0.0f,
            WINDOW_WIDTH_PIXELS,
            WINDOW_HEIGHT_PIXELS,
            {
                { "leftPadding", 16.0f },
                { "rightPadding", 16.0f },
                { "topPadding", 16.0f },
                { "shadow", 0.8f },
                { "align", Text::Align::CENTER }
            }
        );
    };
}