#ifndef LEVEL_H
#define LEVEL_H

#include "level_inventory.hpp"
#include "list.hpp"
#include "object.hpp"
#include "sprites/player.hpp"
#include "sprite.hpp"

namespace BSL
{
    struct Level
    {
        PlayerSprite player;
        struct
        {
            uint_fast16_t w;
            uint_fast16_t h;
        }
        map;
        struct
        {
            float x;
            float y;
        }
        camera;
        uint_fast8_t * collision;
        BSL::GFX::Tilemap object_gfx;
        Object * objects;
        LevelInventory inventory;
        List<Sprite> sprites;

        void init( uint_fast8_t levelid );
        void update( float dt );

        constexpr bool testCollision( uint_fast16_t x, uint_fast16_t y, uint_fast8_t coltype = 0x00 ) const
        {
            const uint_fast16_t i = y * map.w + x;
            return collision[ i ];
        };
    };
}

#endif // LEVEL_H