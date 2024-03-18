#ifndef GAME_H
#define GAME_H

#include "config.hpp"

namespace BSL::Game
{
    struct OWWarp
    {
        uint_fast8_t map;
        uint_fast8_t x;
        uint_fast8_t y;
    };

    union FadeToArgs
    {
        struct
        {
            OWWarp warp;
        }
        ow;
        struct
        {
            uint_fast8_t levelid;
        }
        level;
        void * ptr;
    };

    void init();
    void update( float dt );
    void popState();
    void fadeTo( void ( * constructor )(), FadeToArgs args );
    void pushFadeIn();
    void pushLevelPauseState();
    FadeToArgs getLastFadeToArgs();
    void fadeToOW( OWWarp warp );
}

#endif // GAME_H