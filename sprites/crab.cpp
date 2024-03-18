#include "config.hpp"
#include "level.hpp"
#include "sprite.hpp"

namespace BSL
{
    static constexpr float FALL_SPEED = 0.5f;
    static constexpr float GRAVITY = 6.0f;
    static constexpr float CRAB_START_SPEED = 0.1f;
    static constexpr float CRAB_TOP_SPEED = 0.5f;

    void initCrab( Sprite & sprite, uint_fast16_t x, uint_fast16_t y )
    {
        sprite.type = SpriteType::CRAB;
        auto & crab = sprite.data.crab;
        crab.coords.w = crab.coords.h = 16;
        crab.coords.x = x;
        crab.coords.y = y;
        crab.accx = crab.vx = crab.accy = crab.vy = 0.0f;

        const uint_fast16_t texture = BSL::GFX::loadFileAsTexture( "sprites/common.png" );

        crab.gfx = BSL::GFX::addGraphicSprite
        (
            texture,
            static_cast<int_fast32_t> ( x ),
            static_cast<int_fast32_t> ( y ),
            16,
            16
        );
    };

    void updateCrab( Sprite & sprite, float dt, Level & level )
    {
        auto & crab = sprite.data.crab;
        auto & player = level.player;

        // Handle Y movement.
        crab.accy = FALL_SPEED;
        crab.vy += crab.accy * dt;
        if ( crab.vy > GRAVITY )
        {
            crab.vy = GRAVITY;
        }

        // Handle X movement.
        crab.accx = crab.coords.x > player.right()
            ? -CRAB_START_SPEED
            : ( crab.coords.right() < player.x
                ? CRAB_START_SPEED
                : 0.0f );
        crab.vx += crab.accx * dt;
        if ( crab.vx < -CRAB_TOP_SPEED )
        {
            crab.vx = -CRAB_TOP_SPEED;
        }
        else if ( crab.vx > CRAB_TOP_SPEED )
        {
            crab.vx = CRAB_TOP_SPEED;
        }
        crab.coords.x += crab.vx * dt;

        // Keep in screen boundaries.
        if ( crab.coords.x + crab.vx * dt < 0.0f )
        {
            crab.coords.x = 0.0f;
            crab.vx = 0.0f;
        }
        else if ( crab.coords.right() + crab.vx * dt > level.width * BLOCK_SIZE )
        {
            crab.coords.x = level.width * BLOCK_SIZE - crab.coords.w;
            crab.vx = 0.0f;
        }

        // Handle map collision
        const uint_fast8_t loopcount = std::max( static_cast<uint_fast8_t> ( dt ), static_cast<uint_fast8_t> ( 1 ) );
        const float dti = dt / static_cast<float> ( loopcount );
        for ( uint_fast8_t i = 0; i < loopcount; ++i )
        {
            crab.coords.x += crab.vx * dti;
            crab.coords.y += crab.vy * dti;

            const uint_fast32_t bottomy = static_cast<uint_fast32_t> ( ( crab.coords.y + 15.0 ) / 16.0 );
            const uint_fast32_t yxl = static_cast<uint_fast32_t> ( ( crab.coords.x + 2.0 ) / 16.0 );
            const uint_fast32_t yxr = static_cast<uint_fast32_t> ( ( crab.coords.x + 14.0 ) / 16.0 );
            const uint_fast32_t bottomlefti = bottomy * level.width + yxl;
            const uint_fast32_t bottomrighti = bottomy * level.width + yxr;
            if ( level.collision[ bottomlefti ] == 0x01 || level.collision[ bottomrighti ] == 0x01 )
            {
                crab.coords.y = ( static_cast<float> ( bottomy ) * 16.0f ) - 15.0f;
                crab.vy = 0;
                crab.accy = 0.0f;
            }

            const uint_fast32_t leftx = static_cast<uint_fast32_t> ( crab.coords.x / 16.0 );
            const uint_fast32_t rightx = static_cast<uint_fast32_t> ( crab.coords.right() / 16.0 );
            const uint_fast32_t topyx = static_cast<uint_fast32_t> ( crab.coords.y / 16.0 );
            const uint_fast32_t bottomyx = static_cast<uint_fast32_t> ( ( crab.coords.y + 14.0 ) / 16.0 );
            const uint_fast32_t righttopi = topyx * level.width + rightx;
            const uint_fast32_t rightbottomi = bottomyx * level.width + rightx;
            const uint_fast32_t lefttopi = topyx * level.width + leftx;
            const uint_fast32_t leftbottomi = bottomyx * level.width + leftx;
            if ( level.collision[ righttopi ] == 0x01 || level.collision[ rightbottomi ] == 0x01 )
            {
                crab.coords.x -= crab.coords.right() - ( static_cast<float> ( rightx ) * 16.0f );
                crab.vx *= -1.0f;
            }
            else if ( level.collision[ lefttopi ] == 0x01 || level.collision[ leftbottomi ] == 0x01 )
            {
                crab.coords.x += ( static_cast<float> ( leftx + 1 ) * 16.0f ) - crab.coords.x;
                crab.vx *= -1.0f;
            }
        }

        crab.gfx.setX( crab.coords.x );
        crab.gfx.setY( crab.coords.y );

        
        // Player collision.
        if
        (
            player.x < crab.coords.right() && player.right() > crab.coords.x
            && player.y < crab.coords.bottom() && player.bottom() > ( crab.coords.y + 2.0f )
        )
        {
            player.hurt( level, dt * 1.5f );
        }


        // Update animation.
        crab.animation += dt;
        while ( crab.animation >= 8.0f )
        {
            crab.animation -= 8.0f;
            crab.gfx.toggleFlipX();
        }
    };
};