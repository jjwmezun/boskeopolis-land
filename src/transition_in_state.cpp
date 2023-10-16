#include "game.hpp"
#include "transition_in_state.hpp"

namespace BSL
{
    TransitionInState::TransitionInState()
    :
        speed_ ( 0.0f ),
        opacity_ ( 1.0f )
    {};

    void TransitionInState::update( Game & game, float dt )
    {
        if ( speed_ < 0.1f )
        {
            speed_ += 0.01f;
        }
        opacity_ -= speed_;
        graphic_.setOpacity( opacity_ );

        if ( opacity_ <= 0.0f )
        {
            game.popState();
        }
    };

    void TransitionInState::init( Game & game )
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