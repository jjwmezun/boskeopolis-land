#include "config.hpp"
#include "level.hpp"
#include "sprite.hpp"

namespace BSL
{
    static constexpr float FALL_SPEED = 0.5f;
    static constexpr float GRAVITY = 6.0f;
    static constexpr float APPLE_START_SPEED = 0.1f;
    static constexpr float APPLE_TOP_SPEED = 0.5f;

    static const uint_fast32_t walk_frames[ 4 ] = { 17, 33, 17, 49 };

    void initApple( Sprite & sprite, uint_fast16_t x, uint_fast16_t y, DirX dir )
    {
        sprite.type = SpriteType::APPLE;
        auto & apple = sprite.data.apple;
        apple.alive = true;
        apple.dir = dir;
        apple.coords.w = apple.coords.h = 16;
        apple.coords.x = x;
        apple.coords.y = y;
        apple.accx = apple.vx = apple.accy = apple.vy = 0.0f;

        const uint_fast16_t texture = BSL::GFX::loadFileAsTexture( "sprites/common.png" );

        apple.gfx = BSL::GFX::addGraphicSprite
        (
            texture,
            static_cast<int_fast32_t> ( x + 1 ),
            static_cast<int_fast32_t> ( y ),
            15,
            16,
            {
                { "flipx", dir == DirX::RIGHT },
                { "srcx", 17u }
            }
        );
    };

    void updateApple( Sprite & sprite, float dt, Level & level )
    {
        auto & apple = sprite.data.apple;
        auto & player = level.player;

        // Don’t update if offscreen.
        if (
            apple.coords.x > level.camera.x + WINDOW_WIDTH_PIXELS ||
            apple.coords.right() < level.camera.x ||
            apple.coords.y > level.camera.y + WINDOW_HEIGHT_PIXELS ||
            apple.coords.bottom() < level.camera.y
        )
        {
            return;
        }

        // Handle Y movement.
        apple.accy = FALL_SPEED;
        apple.vy += apple.accy * dt;
        if ( apple.vy > GRAVITY )
        {
            apple.vy = GRAVITY;
        }

        if ( apple.alive )
        {
            // Handle X movement.
            apple.accx = apple.dir == DirX::LEFT ? -APPLE_START_SPEED : APPLE_START_SPEED;
            apple.vx += apple.accx * dt;
            if ( apple.vx < -APPLE_TOP_SPEED )
            {
                apple.vx = -APPLE_TOP_SPEED;
            }
            else if ( apple.vx > APPLE_TOP_SPEED )
            {
                apple.vx = APPLE_TOP_SPEED;
            }
            apple.coords.x += apple.vx * dt;

            // Keep in screen boundaries.
            if ( apple.coords.x + apple.vx * dt < 0.0f )
            {
                apple.coords.x = 0.0f;
                apple.vx = 0.0f;
            }
            else if ( apple.coords.right() + apple.vx * dt > level.map.w * BLOCK_SIZE )
            {
                apple.coords.x = level.map.w * BLOCK_SIZE - apple.coords.w;
                apple.vx = 0.0f;
            }

            // Handle map collision
            const uint_fast8_t loopcount = std::max( static_cast<uint_fast8_t> ( dt ), static_cast<uint_fast8_t> ( 1 ) );
            const float dti = dt / static_cast<float> ( loopcount );
            for ( uint_fast8_t i = 0; i < loopcount; ++i )
            {
                apple.coords.x += apple.vx * dti;
                apple.coords.y += apple.vy * dti;

                const uint_fast32_t bottomy = static_cast<uint_fast32_t> ( ( apple.coords.y + 15.0 ) / 16.0 );
                const uint_fast32_t yxl = static_cast<uint_fast32_t> ( ( apple.coords.x + 2.0 ) / 16.0 );
                const uint_fast32_t yxr = static_cast<uint_fast32_t> ( ( apple.coords.x + 14.0 ) / 16.0 );
                const uint_fast32_t bottomlefti = bottomy * level.map.w + yxl;
                const uint_fast32_t bottomrighti = bottomy * level.map.w + yxr;
                if ( level.collision[ bottomlefti ] == 0x01 || level.collision[ bottomrighti ] == 0x01 )
                {
                    apple.coords.y = ( static_cast<float> ( bottomy ) * 16.0f ) - 15.0f;
                    apple.vy = 0;
                    apple.accy = 0.0f;
                }

                const uint_fast32_t leftx = static_cast<uint_fast32_t> ( apple.coords.x / 16.0 );
                const uint_fast32_t rightx = static_cast<uint_fast32_t> ( apple.coords.right() / 16.0 );
                const uint_fast32_t topyx = static_cast<uint_fast32_t> ( apple.coords.y / 16.0 );
                const uint_fast32_t bottomyx = static_cast<uint_fast32_t> ( ( apple.coords.y + 14.0 ) / 16.0 );
                const uint_fast32_t righttopi = topyx * level.map.w + rightx;
                const uint_fast32_t rightbottomi = bottomyx * level.map.w + rightx;
                const uint_fast32_t lefttopi = topyx * level.map.w + leftx;
                const uint_fast32_t leftbottomi = bottomyx * level.map.w + leftx;
                if ( level.collision[ righttopi ] == 0x01 || level.collision[ rightbottomi ] == 0x01 )
                {
                    apple.coords.x -= apple.coords.right() - ( static_cast<float> ( rightx ) * 16.0f );
                    apple.vx *= -1.0f;
                    apple.dir = DirX::LEFT;
                    apple.gfx.setFlipX( false );
                }
                else if ( level.collision[ lefttopi ] == 0x01 || level.collision[ leftbottomi ] == 0x01 )
                {
                    apple.coords.x += ( static_cast<float> ( leftx + 1 ) * 16.0f ) - apple.coords.x;
                    apple.vx *= -1.0f;
                    apple.dir = DirX::RIGHT;
                    apple.gfx.setFlipX( true );
                }
            }
        }
        else
        {
            apple.coords.y += apple.vy * dt;
        }

        apple.gfx.setX( apple.coords.x );
        apple.gfx.setY( apple.coords.y );

        if ( apple.alive )
        {
            // Player collision.
            if
            (
                player.x < apple.coords.right() && player.right() > apple.coords.x
                && player.y < apple.coords.bottom() && player.bottom() > ( apple.coords.y + 2.0f )
            )
            {
                // If you land on it, it gets defeated & player bounces off it.
                if ( player.vy > 0.0f )
                {
                    apple.alive = false;
                    player.bounce();
                }
                // Otherwise, the player gets hurt.
                else
                {
                    player.hurt( level, dt * 1.5f );
                }
            }


            // Update animation.
            apple.animation += dt;
            while ( apple.animation >= 8.0f )
            {
                apple.animation -= 8.0f;
                ++apple.walk_frame;
                if ( apple.walk_frame > 3 )
                {
                    apple.walk_frame = 0;
                }
                apple.gfx.setSrcX( walk_frames[ apple.walk_frame ] );
            }
        }
    };
};