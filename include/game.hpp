#ifndef GAME_H
#define GAME_H

#include "config.hpp"

namespace BSL::Game
{
    void init();
    void update( float dt );
    void popState();
}

#endif // GAME_H