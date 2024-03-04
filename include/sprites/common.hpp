#ifndef SPRITE_COMMON_H
#define SPRITE_COMMON_H

#include "gfx.hpp"

namespace BSL
{
    struct SpriteCoords
    {
        float x;
        float y;
        float w;
        float h;

        constexpr float right() const
        {
            return x + w;
        };

        constexpr float bottom() const
        {
            return y + h;
        };
    };

    struct SpriteBasic
    {
        SpriteCoords coords;
        float accx;
        float accy;
        float vx;
        float vy;
    };
};

#endif // SPRITE_COMMON_H