#ifndef OW_LEVEL_MENU_H
#define OW_LEVEL_MENU_H

#include "gfx.hpp"

namespace BSL
{
    struct OWLevelMenu
    {
        static constexpr uint_fast8_t OPTIONCOUNT = 2;

        bool confirmlock;
        uint_fast8_t levelid;
        uint_fast8_t selection;
        struct
        {
            GFX::Rect selection;
            GFX::Text optiontext[ OPTIONCOUNT ];
        }
        gfx;

        void init( uint_fast8_t lvid );
        void update( float dt );
        void updateSelector( uint_fast8_t newselection );
    };
};

#endif // OW_LEVEL_MENU_H