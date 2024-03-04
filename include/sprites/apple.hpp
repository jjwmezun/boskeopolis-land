#ifndef SPRITE_APPLE_H
#define SPRITE_APPLE_H

#include "dir.hpp"
#include "gfx.hpp"
#include "sprites/common.hpp"

namespace BSL
{
    class Level;
    class Sprite;

    struct AppleSprite
    {
        bool alive;
        DirX dir;
        BSL::GFX::Sprite gfx;
        uint_fast8_t walk_frame;
        float accx;
        float accy;
        float vx;
        float vy;
        float animation;
        SpriteCoords coords;
    };

    void initApple( Sprite & sprite, uint_fast16_t x, uint_fast16_t y, DirX dir );
    void updateApple( Sprite & sprite, float dt, Level & level );
};

#endif // SPRITE_APPLE_H