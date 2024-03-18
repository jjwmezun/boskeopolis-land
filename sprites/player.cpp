#include <cmath>
#include "config.hpp"
#include "controls.hpp"
#include "level.hpp"
#include "sprites/player.hpp"

namespace BSL
{
    static constexpr float START_SPEED = 0.15f;
    static constexpr float FALL_SPEED = 0.5f;
    static constexpr float GRAVITY = 6.0f;
    static constexpr float JUMP_ACC = 0.2f;
    static constexpr float JUMP_INIT = 2.0f;
    static constexpr float JUMP_MAX = 4.75f;
    static constexpr float autumn_walk_frames[ 4 ] = { 0.0f, 16.0f, 0.0f, 32.0f };

    void PlayerSprite::init()
    {
        hp = 20.0f;
        resetGFX();
    };

    void PlayerSprite::resetGFX()
    {
        uint_fast16_t player_texture = GFX::loadFileAsTexture( "sprites/autumn.png" );
        gfx = GFX::addGraphicSprite
        (
            player_texture,
            static_cast<int_fast32_t> ( x ),
            static_cast<int_fast32_t> ( y ),
            16,
            26
        );
    };

    void PlayerSprite::update( float dt, Level & level )
    {
        // Check if running.
        const float start_speed = Controls::heldRun() ? START_SPEED * 2.0f : START_SPEED;
        const float max_speed = Controls::heldRun() ? MAX_SPEED * 2.0f : MAX_SPEED;

        // Autumn Y movement.
        // Falling & Jumping
        const bool going_fast = isGoingFast();
        const float gravity = BSL::Controls::heldJump() ? GRAVITY / 1.5f : GRAVITY;
        const float max_jump = going_fast ? JUMP_MAX * 1.1f : JUMP_MAX;
        const bool can_start_jump = jump_padding > 0.0f &&
            !jump_lock &&
            BSL::Controls::heldJump();

        // Continue jump.
        if ( is_jumping )
        {
            if ( BSL::Controls::heldJump() )
            {
                accy = -JUMP_ACC;
            }
            else
            {
                is_jumping = false;
                accy = 0.0f;
            }
        }
        // Start jump.
        else if ( can_start_jump )
        {
            is_jumping = true;
            vy = -JUMP_INIT;
            accy = -JUMP_ACC;
        }
        // Else, fall.
        else
        {
            accy = FALL_SPEED;
        }

        // Update player y speed.
        vy += accy * dt;
        if ( vy > gravity )
        {
            vy = gravity;
        }
        else if ( vy < -max_jump )
        {
            vy = -max_jump;
            is_jumping = false;
            accy = 0.0f;
        }

        // Reset on ground.
        on_ground = false;

        // Update jump padding.
        jump_padding = std::max( 0.0f, jump_padding - 1.0f * dt);

        // Update jump lock.
        jump_lock = Controls::heldJump();


        // Update Autumn.
        static constexpr float PLAYER_HANDLING = 0.25f;

        // Autumn X movement.
        if ( BSL::Controls::heldRight() )
        {
            accx = start_speed;
            gfx.setFlipX( true );
        }
        else if ( BSL::Controls::heldLeft() )
        {
            accx = -start_speed;
            gfx.setFlipX( false );
        }
        else
        {
            accx = 0.0f;
        }

        vx += accx * dt;

        if ( accx == 0.0f )
        {
            vx /= ( 1.0f + PLAYER_HANDLING * dt );
        }

        if ( vx > max_speed )
        {
            vx = max_speed;
        }
        else if ( vx < -max_speed )
        {
            vx = -max_speed;
        }


        // Object & other collision.
        const uint_fast32_t otopy = static_cast<uint_fast32_t> ( ( y + 2.0 ) / 16.0 );
        const uint_fast32_t obottomy = static_cast<uint_fast32_t> ( ( y + 23.0 ) / 16.0 );
        const uint_fast32_t oleftx = static_cast<uint_fast32_t> ( ( x + 2.0 ) / 16.0 );
        const uint_fast32_t orightx = static_cast<uint_fast32_t> ( ( x + 14.0 ) / 16.0 );
        const uint_fast32_t oilist[ 4 ] =
        {
            otopy * level.width + oleftx,
            otopy * level.width + orightx,
            obottomy * level.width + oleftx,
            obottomy * level.width + orightx,
        };
        for ( uint_fast8_t i = 0; i < 4; ++i )
        {
            const uint_fast32_t & oi = oilist[ i ];

            switch ( level.collision[ oi ] )
            {
                case ( 0x04 ):
                {
                    level.warp();

                    // Interrupt update & prevent multiple warps.
                    return;
                }
                break;
            }

            // Object collision.
            switch ( level.objects[ oi ].type )
            {
                case ( ObjectType::MONEY ):
                {
                    // Get money.
                    level.inventory.addMoney( level.objects[ oi ].data.money.amount );

                    // Remove object.
                    level.objects[ oi ].type = ObjectType::__NULL;
                    level.object_gfx.removeTile( oi );
                    level.maps[ level.current_map ].objects[ oi ] = 0;
                }
                break;
            }
        }

        // Main collision
        if ( x + vx * dt < 0.0f )
        {
            x = 0.0f;
            vx = 0.0f;
        }
        else if ( x + BLOCK_SIZE + vx * dt > level.width * BLOCK_SIZE )
        {
            x = level.width * BLOCK_SIZE - BLOCK_SIZE;
            vx = 0.0f;
        }


        // Map collision
        const uint_fast8_t loopcount = std::max( static_cast<uint_fast8_t> ( dt ), static_cast<uint_fast8_t> ( 1 ) );
        const float dti = dt / static_cast<float> ( loopcount );
        for ( uint_fast8_t i = 0; i < loopcount; ++i )
        {
            x += vx * dti;
            y += vy * dti;

            const uint_fast32_t topy = static_cast<uint_fast32_t> ( getTopBoundary() / 16.0 );
            const uint_fast32_t bottomy = static_cast<uint_fast32_t> ( getBottomBoundary() / 16.0 );
            const uint_fast32_t yx = static_cast<uint_fast32_t> ( ( x + 8.0 ) / 16.0 );
            const uint_fast32_t yxl = static_cast<uint_fast32_t> ( ( x + 2.0 ) / 16.0 );
            const uint_fast32_t yxr = static_cast<uint_fast32_t> ( ( x + 14.0 ) / 16.0 );
            const uint_fast32_t toplefti = topy * level.width + yx;
            const uint_fast32_t toprighti = topy * level.width + yx;
            const uint_fast32_t bottomlefti = bottomy * level.width + yxl;
            const uint_fast32_t bottomrighti = bottomy * level.width + yxr;
            if ( level.collision[ toplefti ] == 0x01 || level.collision[ toprighti ] == 0x01 )
            {
                y += ( static_cast<float> ( topy + 1 ) * 16.0f ) - getTopBoundary();
                vy *= -0.25f;
                accy = 0.0f;
                is_jumping = false;
            }
            else if ( level.collision[ bottomlefti ] == 0x01 || level.collision[ bottomrighti ] == 0x01 )
            {
                y = ( static_cast<float> ( bottomy ) * 16.0f ) - 24.0f;
                vy = 0;
                accy = 0.0f;
                on_ground = true;
                jump_padding = isGoingFast() ? 8.0f : 2.0f;
            }
            const uint_fast32_t leftx = static_cast<uint_fast32_t> ( getLeftBoundary() / 16.0 );
            const uint_fast32_t rightx = static_cast<uint_fast32_t> ( getRightBoundary() / 16.0 );
            const uint_fast32_t topyx = static_cast<uint_fast32_t> ( ( getTopBoundary() + 3.0 ) / 16.0 );
            const uint_fast32_t bottomyx = static_cast<uint_fast32_t> ( ( getBottomBoundary() - 3.0 ) / 16.0 );
            const uint_fast32_t righttopi = topyx * level.width + rightx;
            const uint_fast32_t rightbottomi = bottomyx * level.width + rightx;
            const uint_fast32_t lefttopi = topyx * level.width + leftx;
            const uint_fast32_t leftbottomi = bottomyx * level.width + leftx;
            if ( level.collision[ righttopi ] == 0x01 || level.collision[ rightbottomi ] == 0x01 )
            {
                x -= getRightBoundary() - ( static_cast<float> ( rightx ) * 16.0f );
                vx *= -0.25f;
            }
            else if ( level.collision[ lefttopi ] == 0x01 || level.collision[ leftbottomi ] == 0x01 )
            {
                x += ( static_cast<float> ( leftx + 1 ) * 16.0f ) - getLeftBoundary();
                vx *= -0.25f;
            }
        }

        // Update final position.
        gfx.setX( x );
        gfx.setY( y );


        // Update animation.
        if ( !on_ground )
        {
            gfx.setSrcX( 48.0f );
            gfx.setSrcY( 0.0f );
        }
        else
        {
            // Update walking animation if not stopped.
            if ( std::abs( accx ) > 0.01f )
            {
                if ( ( walk_timer += dt ) >= 8.0f )
                {
                    walk_timer -= 8.0f;
                    ++walk_frame;
                    if ( walk_frame > 3 )
                    {
                        walk_frame = 0;
                    }
                }
                gfx.setSrcX( autumn_walk_frames[ walk_frame ] );
                gfx.setSrcY( 0.0f );
            }
            else
            {
                gfx.setSrcX( 0.0f );
                gfx.setSrcY( 0.0f );
            }
        }


        // Update invincibility.
        if ( invincibility > 0.0f )
        {
            invincibility -= dt;
            if ( invincibility < 0.0f )
            {
                invincibility = 0.0f;
            }
        }

        const float invisible = static_cast<uint_fast8_t> ( invincibility / 8.0 ) % 2 == 1 ? 0.0f : 1.0f;
        gfx.setOpacity( invisible );
    };

    void PlayerSprite::hurt( Level & level, float amount )
    {
        level.inventory.hurt( amount );
        if ( invincibility <= 0.0f )
        {
            invincibility = 16.0f;
        }
    };

    void PlayerSprite::bounce()
    {
        is_jumping = true;
        vy = -JUMP_INIT;
        accy = -JUMP_ACC;
    };
};