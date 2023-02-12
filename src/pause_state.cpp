#include "config.hpp"
#include "input.hpp"
#include "game.hpp"
#include "nasringine/nasr_localization.h"
#include "pause_state.hpp"
#include "title_state.hpp"
#include <cstring>

namespace BSL
{
    PauseState::PauseState() : opacity_ ( 0.0f ), boxgfx_ ( 0 ) {};

    void PauseState::update( Game & game, float dt )
    {
        if ( Input::pressedJump() )
        {
            game.changeState( std::make_unique<TitleState>() );
        }
        else if ( Input::pressedMenu() )
        {
            game.popState();
        }
        if ( opacity_ < 1.0f )
        {
            opacity_ += 0.1f * dt;
            boxgfx_.setOpacity( opacity_ );
        }
    };

    void PauseState::init( Game & game )
    {
        game.render().addRect
        (
            50.0f,
            50.0f,
            50.0f,
            50.0f,
            1
        );

        // Translate options.
        const OptionList option_text = ArrayMap( OPTIONS, []( const char * option ) { return Nasr_x( option, "Pause Menu", "boskeopolis-land" ); } );

        // Find longest option name.
        const int max_width = ArrayReduce( option_text, []( int acc, const char * current, unsigned int index ) { return std::max( acc, static_cast<int>( std::strlen( current ) ) ); }, 0 );

        const float text_height = static_cast<float>( 12 * ( NUM_O_OPTIONS - 1 ) ) + 8.0f;
        const float text_width = static_cast<float>( 8 * max_width );

        boxgfx_ = game.render().addMenuBox
        (
            Text::Align::CENTER,
            Text::Valign::MIDDLE,
            text_width + 24.0f,
            text_height + 24.0f,
            {
                { "opacity", 0.0f }
            }
        );

        float y = ( static_cast<float>( WINDOW_HEIGHT_PIXELS ) - text_height ) / 2.0f;
        const float x = ( static_cast<float>( WINDOW_WIDTH_PIXELS ) - text_width ) / 2.0f;
        for ( unsigned int i = 0; i < NUM_O_OPTIONS; ++i )
        {
            textgfx_[ i ] = game.render().addTextGradient
            (
                std::string( option_text.at( i ) ),
                Dir::XY::DOWN,
                96,
                64,
                x,
                y,
                text_width,
                12.0f,
            {
                { "opacity", 0.0f }
            }
            );
            y += 12.0f;
        }
    };
}