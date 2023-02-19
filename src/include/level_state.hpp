#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "clock.hpp"
#include "game_state.hpp"
#include "input_controller.hpp"
#include "map.hpp"
#include "sprite.hpp"

namespace BSL
{
    class LevelState : public GameState
    {
        public:
            LevelState();
            void update( Game & game, float dt ) override;
            void init( Game & game ) override;

        private:
            Map map_;
            Sprite autumn_;
            InputController input_;
            Clock clock_;
    };
}

#endif // LEVEL_STATE_H