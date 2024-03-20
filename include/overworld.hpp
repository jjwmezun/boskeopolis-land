#ifndef OVERWORLD_H
#define OVERWORLD_H

#include "config.hpp"
#include "gfx.hpp"
#include "ow_warp.hpp"

namespace BSL
{
    static constexpr unsigned int WATERTILEW = 8;
    static constexpr unsigned int WATERTILEH = 4;
    static constexpr unsigned int WATERW = WINDOW_WIDTH_PIXELS + WATERTILEW;
    static constexpr unsigned int WATERH = WATERTILEH;
    static constexpr unsigned int WATERROWS = ( WINDOW_HEIGHT_PIXELS - 7 * 8 ) / 4 + 3;
    static constexpr unsigned int BGGFXCOUNT = 4;
    static constexpr float OWWINDOWH = 248.0f;
    static constexpr float CAMERA_RIGHT_EDGE = WINDOW_WIDTH_PIXELS * 0.667f;
    static constexpr float CAMERA_LEFT_EDGE = WINDOW_WIDTH_PIXELS * 0.333f;
    static constexpr float CAMERA_BOTTOM_EDGE = OWWINDOWH * 0.667f;
    static constexpr float CAMERA_TOP_EDGE = OWWINDOWH * 0.333f;
    static constexpr unsigned int OWLEVELOPEN_OPTIONCOUNT = 2;
    static constexpr int OWLVOPEN_MENUX = static_cast<int> ( ( WINDOW_WIDTH_PIXELS - 160 ) / 2.0 );
    static constexpr int OWLVOPEN_MENUY = static_cast<int> ( ( WINDOW_HEIGHT_PIXELS - 32 ) / 2.0 );

    struct Overworld
    {
        BSL::GFX::RawSprite water[ WATERROWS ];
        float waterxodd;
        float waterxeven;
        struct
        {
            BSL::GFX::RawSprite gfx[ BGGFXCOUNT ];
            float x;
            float y;
        }
        bg;
        struct
        {
            BSL::GFX::Sprite gfx;
            float accx;
            float accy;
            float vx;
            float vy;
            float x;
            float y;

            constexpr inline float getCenterX()
            {
                return x + 8.0f;
            }

            constexpr inline float getCenterY()
            {
                return y + 8.0f;
            }

            constexpr inline float getLeftBoundary()
            {
                return x + 5.0f;
            }

            constexpr inline float getRightBoundary()
            {
                return x + 11.0f;
            }

            constexpr inline float getTopBoundary()
            {
                return y + 4.0f;
            }

            constexpr inline float getBottomBoundary()
            {
                return y + 12.0f;
            }
        }
        autumn;
        struct
        {
            float x;
            float y;
        }
        camera;
        struct
        {
            unsigned int w;
            unsigned int h;
            uint_fast8_t * collision;
            uint_fast8_t * levels;
            OWWarp * warps;
            uint_fast8_t warpcount;
        }
        map;
        struct
        {
            BSL::GFX::Text lvname;
            BSL::GFX::Counter gemscore;
            BSL::GFX::Timer timescore;
        }
        ui;
        uint_fast8_t prev_level;
        uint_fast8_t current_level;

        void init( OWWarp inwarp );
        void destroy();
        void update( float dt );
        void updateCamera();
        void updateOWAnimation( float dt );
    };
}

#endif // OVERWORLD_H