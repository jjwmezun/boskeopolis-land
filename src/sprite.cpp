#include "config.hpp"
#include "map.hpp"
#include "nasringine/nasr.h"
#include "sprite.hpp"
#include "controller.hpp"

#include <iostream>

constexpr float START_SPEED = 0.25f;
constexpr float MAX_SPEED = 2.5f;
constexpr float FALL_SPEED = 0.5f;
constexpr float GRAVITY = 6.0f;
constexpr float JUMP_ACC = 0.5f;
constexpr float JUMP_INIT = 1.0f;
constexpr float JUMP_MAX = 6.5f;

namespace BSL
{
    Sprite::Sprite() :
        pos_ ( 64.0f, 64.0f, 16.0f, 25.0f ),
        accx_ ( 0.0f ),
        vx_ ( 0.0f ),
        prevx_ ( 0.0f ),
        accy_ ( 0.0f ),
        vy_ ( 0.0f ),
        prevy_ ( 0.0f ),
        is_jumping_ ( false ),
        jump_lock_ ( false ),
        on_ground_ ( false ),
        jump_padding_ ( 0.0f )
    {
        int autumntexture = NasrLoadFileAsTexture( "assets/graphics/sprites/autumn.png" );
        NasrRect autumnsrc { 0.0f, 0.0f, 16.0f, 25.0f };
        NasrRect autumndest { 64.0f, 64.0f, 16.0f, 25.0f };
        graphic_ = NasrGraphicsAddSprite
        (
            0,
            0,
            0,
            autumntexture,
            autumnsrc,
            autumndest,
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            0
        );
    };

    void Sprite::update( float dt, const Controller & controller, Map & map )
    {
        if ( controller.heldMenu() )
        {
            pos_.x = 64.0f;
            pos_.y = 64.0f;
            accx_ = 0.0f;
            vx_ = 0.0f;
            prevx_ = 0.0f;
            accy_ = 0.0f;
            vy_ = 0.0f;
            prevy_ = 0.0f;
            is_jumping_ = false;
            jump_lock_ = false;
            on_ground_ = false;
        }


        // Horizontal Movement
        const float start_speed = controller.heldRun() ? START_SPEED * 2.0f : START_SPEED;
        const float max_speed = controller.heldRun() ? MAX_SPEED * 2.0f : MAX_SPEED;
        if ( controller.heldRight() )
        {
            accx_ = start_speed;
        }
        else if ( controller.heldLeft() )
        {
            accx_ = -start_speed;
        }
        else
        {
            accx_ = 0.0f;
        }

        vx_ += accx_ * dt;
        if ( vx_ > max_speed )
        {
            vx_ = max_speed;
        }
        else if ( vx_ < -max_speed )
        {
            vx_ = -max_speed;
        }
        if ( accx_ == 0.0f )
        {
            vx_ /= ( 1.0f + 0.2f * dt );
        }
        pos_.x += vx_ * dt;
        

        // Falling & Jumping
        const bool going_fast = std::abs( vx_ ) >= MAX_SPEED * 1.5f;
        const float gravity = controller.heldJump() ? GRAVITY / 1.5f : GRAVITY;
        const float max_jump = going_fast ? JUMP_MAX * 1.1f : JUMP_MAX;
        if ( jump_padding_ > 0.0f )
        {
            if ( !jump_lock_ && controller.heldJump() )
            {
                is_jumping_ = true;
                vy_ = -JUMP_INIT;
                accy_ = -JUMP_ACC;
            }
        }
        else
        {
            if ( is_jumping_ )
            {
                if ( controller.heldJump() )
                {
                    accy_ = -JUMP_ACC;
                }
                else
                {
                    is_jumping_ = false;
                    accy_ = 0.0f;
                }
            }
            else
            {
                accy_ = FALL_SPEED;
            }
        }

        vy_ += accy_ * dt;
        if ( vy_ > gravity )
        {
            vy_ = gravity;
        }
        else if ( vy_ < -max_jump )
        {
            vy_ = -max_jump;
            is_jumping_ = false;
            accy_ = 0.0f;
        }

        pos_.y += vy_ * dt;






        // Collision
        /*
        if ( isOnGround() )
        {
            pos_.y = 320.0f - 25.0f;
        }
        */
        on_ground_ = false;
        jump_padding_ = std::max( 0.0f, jump_padding_ - 1.0f * dt);

        // Handle downward collision.
        const int yl = pixelsToBlocks( pos_.x + 2.0f );
        const int yr = pixelsToBlocks( pos_.right() - 2.0f );
        if ( vy_ > 0.0f )
        {
            const int by = pixelsToBlocks( pos_.bottom() - 1.0f );
            const bool blc = map.testCollision( yl, by );
            const bool brc = map.testCollision( yr, by );
            if ( blc || brc )
            {
                pos_.y -= pos_.bottom() - 1.0f - blocksToPixels( by );
                on_ground_ = true;
                jump_padding_ = going_fast ? 8.0f : 2.0f;
            }
        }

        // Handle upward collision.
        const int ty = pixelsToBlocks( pos_.y );
        const bool tlc = map.testCollision( yl, ty );
        const bool trc = map.testCollision( yr, ty );
        if ( tlc || trc )
        {
            pos_.y = blocksToPixels( ty + 1 );
            vy_ *= -0.25f;
            accy_ = 0.0f;
            is_jumping_ = false;
        }

        // Handle right collision.
        const int rx = pixelsToBlocks( pos_.right() );
        const int xt = pixelsToBlocks( pos_.y + 2.0f );
        const int xb = pixelsToBlocks( pos_.bottom() - 4.0f );
        const bool rtc = map.testCollision( rx, xt );
        const bool rbc = map.testCollision( rx, xb );
        if ( rtc || rbc )
        {
            pos_.x -= pos_.right() - blocksToPixels( rx );
            vx_ *= -0.25f;
            accx_ = 0.0f;
        }
        const int lx = pixelsToBlocks( pos_.x );
        const bool ltc = map.testCollision( lx, xt );
        const bool lbc = map.testCollision( lx, xb );
        if ( ltc || lbc )
        {
            pos_.x = blocksToPixels( lx + 1 );
            vx_ *= -0.25f;
            accx_ = 0.0f;
        }









        // Graphics
        if ( pos_.x != prevx_ )
        {
            NasrGraphicsSpriteSetDestX( graphic_, pos_.x );
        }
        if ( pos_.y != prevy_ )
        {
            NasrGraphicsSpriteSetDestY( graphic_, pos_.y );
        }


        // End
        prevx_ = pos_.x;
        prevy_ = pos_.y;
        jump_lock_ = controller.heldJump();

    };

    bool Sprite::isOnGround() const
    {
        return on_ground_;
    };
}