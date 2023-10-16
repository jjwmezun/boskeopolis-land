#include "game.hpp"
#include "transition_in_state.hpp"
#include "transition_out_message_state.hpp"

namespace BSL
{
    TransitionOutMessageState::TransitionOutMessageState( int message )
    :
        speed_ ( 0.0f ),
        opacity_ ( 0.0f ),
        message_ ( message )
    {};

    void TransitionOutMessageState::update( Game & game, float dt )
    {
        if ( speed_ < 0.1f )
        {
            speed_ += 0.01f;
        }
        opacity_ += speed_;
        graphic_.setOpacity( opacity_ );

        if ( opacity_ >= 1.0f )
        {
            game.popState( message_ );
            game.pushState( std::make_unique<TransitionInState> () );
        }
    };

    void TransitionOutMessageState::init( Game & game )
    {
        graphic_ = game.render().addScreen
        (
            255,
            1.0f,
            1.0f,
            Layer::SUPER,
            opacity_
        );
    };
}