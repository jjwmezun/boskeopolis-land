#include "game.hpp"
#include "transition_in_state.hpp"
#include "transition_out_state.hpp"

namespace BSL
{
    TransitionOutState::TransitionOutState( std::unique_ptr<GameState> && state )
    :
        state_ ( std::move( state ) ),
        speed_ ( 0.0f ),
        opacity_ ( 0.0f )
    {};

    void TransitionOutState::update( Game & game, float dt )
    {
        if ( speed_ < 0.1f )
        {
            speed_ += 0.01f;
        }
        opacity_ += speed_;
        graphic_.setOpacity( opacity_ );

        if ( opacity_ >= 1.0f )
        {
            game.changeState( std::unique_ptr<GameState>( state_.release() ) );
            game.pushState( std::make_unique<TransitionInState> () );
        }
    };

    void TransitionOutState::init( Game & game )
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