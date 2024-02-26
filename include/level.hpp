#ifndef LEVEL_H
#define LEVEL_H

namespace BSL
{
    static constexpr float MAX_SPEED = 2.0f;

    struct Level
    {
        struct
        {
            bool on_ground;
            bool is_jumping;
            bool jump_lock;
            uint_fast8_t walk_frame;
            float x;
            float y;
            float vx;
            float accx;
            float vy;
            float accy;
            float jump_padding;
            float walk_timer;
            BSL::GFX::Sprite gfx;

            constexpr inline float getCenterX() const
            {
                return x + 8.0f;
            };

            constexpr inline float getCenterY() const
            {
                return y + 13.0f;
            };

            constexpr inline float getLeftBoundary() const
            {
                return x;
            };

            constexpr inline float getRightBoundary() const
            {
                return x + 16.0f;
            };

            constexpr inline float getTopBoundary() const
            {
                return y + 4.0f;
            };

            constexpr inline float getBottomBoundary() const
            {
                return y + 24.0f;
            };

            constexpr bool isGoingFast() const
            {
                return std::abs( vx ) >= MAX_SPEED * 1.5f;
            };

            constexpr float bottom() const
            {
                return y + 26.0f;
            };

            constexpr float right() const
            {
                return x + 16.0f;
            };
        }
        player;
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