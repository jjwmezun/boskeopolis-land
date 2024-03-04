#ifndef SPRITE_H
#define SPRITE_H

#include "sprites/apple.hpp"
#include "sprites/crab.hpp"
#include "gfx.hpp"

namespace BSL
{
    class Level;

    enum class SpriteType
    {
        CRAB,
        APPLE
    };

    struct Sprite
    {
        SpriteType type;
        union
        {
            CrabSprite crab;
            AppleSprite apple;
        }
        data;

        void init( uint_fast16_t index, uint_fast16_t x, uint_fast16_t y );
        void update( float dt, Level & level );
    };
};

#endif // SPRITE_H