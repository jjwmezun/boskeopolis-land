#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "game_state.hpp"
#include "input_controller.hpp"
#include "level.hpp"

namespace BSL
{
    class LevelState : public GameState
    {
        public:
            LevelState();
            void update( Game & game, float dt ) override;
            void init( Game & game ) override;
            void backToState( Game & game, int message ) override;

        private:
            InputController input_;
            Level level_;
    };
}

#endif // LEVEL_STATE_H