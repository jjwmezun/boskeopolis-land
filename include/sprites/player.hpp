#ifndef SPRITE_PLAYER_H
#define SPRITE_PLAYER_H

#include <cstdint>

namespace BSL
{
    static constexpr float MAX_SPEED = 2.0f;

    class Level;

    struct PlayerSprite
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
        float invincibility;
        float hp;

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

        void init();
        void resetGFX();
        void update( float dt, Level & level );
        void hurt( Level & level, float amount );
        void bounce();
    };
};

#endif // SPRITE_PLAYER_H