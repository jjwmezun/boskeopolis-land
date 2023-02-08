#ifndef TITLE_H
#define TITLE_H

#include "game_state.hpp"

namespace BSL
{
    class TitleState : public GameState
    {
        public:
            void update( Game & game, float dt ) override;
            void init( Game & game ) override;
    };
}

#endif // TITLE_H