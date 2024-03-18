#ifndef LEVEL_H
#define LEVEL_H

#include "level_inventory.hpp"
#include "list.hpp"
#include "object.hpp"
#include "sprites/player.hpp"
#include "sprite.hpp"

namespace BSL
{
    struct TileLayer
    {
        uint_fast16_t * tiles;
        float scrollx;
        float scrolly;
        int offsetx;
        int offsety;
    };

    struct ImageLayer
    {
        char * src;
        bool repeatx;
        bool repeaty;
        float scrollx;
        float scrolly;
        int offsetx;
        int offsety;
    };

    struct LevelWarp
    {
        uint_fast8_t map;
        uint_fast16_t warpx;
        uint_fast16_t warpy;
        float left;
        float right;
        float top;
        float bottom;
    };

    struct Map
    {
        uint_fast16_t width;
        uint_fast16_t height;
        uint_fast8_t * collision;
        uint_fast16_t * sprites;
        uint_fast16_t * objects;
        List<TileLayer> tiles;
        List<ImageLayer> images;
        List<LevelWarp> warps;
    };

    struct Level
    {
        PlayerSprite player;
        uint_fast16_t width;
        uint_fast16_t height;
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
        Map * maps;
        uint_fast8_t mapcount;
        uint_fast8_t current_map;
        const LevelWarp * current_warp;

        void init( uint_fast8_t levelid );
        void destroy();
        void update( float dt );
        void updateCamera();
        void generateLevelData();
        void warp();
        void finishWarp();

        constexpr bool testCollision( uint_fast16_t x, uint_fast16_t y, uint_fast8_t coltype = 0x00 ) const
        {
            const uint_fast16_t i = y * width + x;
            return collision[ i ];
        };
    };
}

#endif // LEVEL_H