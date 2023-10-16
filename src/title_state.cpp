#include "config.hpp"
#include "game.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "title_state.hpp"
#include "transition_out_state.hpp"

namespace BSL
{
    void TitleState::update( Game & game, float dt )
    {
        if ( Input::pressedJump() )
        {
            game.pushState( std::make_unique<TransitionOutState> ( std::make_unique<LevelState> () ) );
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
            1.0f,
            1.0f
        );

        game.render().addText
        (
            {
                { "text", std::string( "Boskeopolis Land" ) },
                { "dir", Dir::XY::DOWN },
                { "type", "gradient" },
                { "color1", 224 },
                { "color2", 64 },
                { "leftPadding", 16.0f },
                { "rightPadding", 16.0f },
                { "topPadding", 16.0f },
                { "shadow", 0.8f },
                { "align", Text::Align::CENTER }
            }
        );
    };
}