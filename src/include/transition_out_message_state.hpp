#ifndef TRANSITION_OUT_MESSAGE_STATE_H
#define TRANSITION_OUT_MESSAGE_STATE_H

#include "game_state.hpp"
#include <memory>
#include "renderer.hpp"

namespace BSL
{
    class TransitionOutMessageState : public GameState
    {
        public:
            TransitionOutMessageState( int message );
            void update( Game & game, float dt ) override;
            void init( Game & game ) override;

        private:
            RectGraphic graphic_;
            float speed_;
            float opacity_;
            int message_;
    };
}

#endif // TRANSITION_OUT_MESSAGE_STATE_H