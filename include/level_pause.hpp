#ifndef LEVEL_PAUSE_H
#define LEVEL_PAUSE_H

#include "gfx.hpp"

namespace BSL
{
    struct LevelPause
    {
        static constexpr uint_fast8_t OPTIONCOUNT = 2;

        bool confirmlock;
        uint_fast8_t selection;
        struct
        {
            GFX::Rect selection;
            GFX::Text optiontext[ OPTIONCOUNT ];
        }
        gfx;

        void init();
        void update( float dt );
        void updateSelector( uint_fast8_t newselection );
    };
};

#endif // LEVEL_PAUSE_H