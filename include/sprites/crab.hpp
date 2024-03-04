#ifndef SPRITE_CRAB_H
#define SPRITE_CRAB_H

#include "gfx.hpp"
#include "sprites/common.hpp"

namespace BSL
{
    class Level;
    class Sprite;

    struct CrabSprite
    {
        BSL::GFX::Sprite gfx;
        SpriteCoords coords;
        float accx;
        float accy;
        float vx;
        float vy;
        float animation;
    };

    void initCrab( Sprite & sprite, uint_fast16_t x, uint_fast16_t y );
    void updateCrab( Sprite & sprite, float dt, Level & level );
};

#endif // SPRITE_CRAB_H