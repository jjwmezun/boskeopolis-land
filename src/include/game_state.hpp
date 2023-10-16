#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace BSL
{
    class Game;

    class GameState
    {
        public:
            virtual ~GameState() {};
            virtual void update( Game & game, float dt ) = 0;
            virtual void init( Game & game ) = 0;
            virtual inline void backToState( Game & game, int message = 0 ) {};
    };
}

#endif // GAME_STATE_H