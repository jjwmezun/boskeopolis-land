#ifndef GAME_H
#define GAME_H

#include "config.hpp"
#include "ow_warp.hpp"

namespace BSL::Game
{
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
    void pushLvMessageState();
    void pushOWLevelOpenMenu( uint_fast8_t levelid );
}

#endif // GAME_H