#ifndef TRANSITION_OUT_STATE_H
#define TRANSITION_OUT_STATE_H

#include "game_state.hpp"
#include <memory>
#include "renderer.hpp"

namespace BSL
{
    class TransitionOutState : public GameState
    {
        public:
            TransitionOutState( std::unique_ptr<GameState> && state );
            void update( Game & game, float dt ) override;
            void init( Game & game ) override;

        private:
            std::unique_ptr<GameState> state_;
            RectGraphic graphic_;
            float speed_;
            float opacity_;
    };
}

#endif // TRANSITION_OUT_STATE_H