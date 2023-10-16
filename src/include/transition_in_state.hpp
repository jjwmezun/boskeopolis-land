#ifndef TRANSITION_IN_STATE_H
#define TRANSITION_IN_STATE_H

#include "game_state.hpp"
#include <memory>
#include "renderer.hpp"

namespace BSL
{
    class TransitionInState : public GameState
    {
        public:
            TransitionInState();
            void update( Game & game, float dt ) override;
            void init( Game & game ) override;

        private:
            RectGraphic graphic_;
            float speed_;
            float opacity_;
    };
}

#endif // TRANSITION_IN_STATE_H