#include "nasringine/nasr.h"
#include "sprite.hpp"

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
        accx_ ( 0.0f ),
        vx_ ( 0.0f ),
        x_ ( 64.0f ),
        prevx_ ( 0.0f ),
        accy_ ( 0.0f ),
        vy_ ( 0.0f ),
        y_ ( 64.0f ),
        prevy_ ( 0.0f ),
        jump_start_ ( 0.0f ),
        is_jumping_ ( false ),
        jump_lock_ ( false )
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

    void Sprite::update( float dt, Controller & controller )
    {
        const float gravity = controller.heldJump() ? GRAVITY / 1.5f : GRAVITY;
        const float start_speed = controller.heldRun() ? START_SPEED * 2.0f : START_SPEED;
        const float max_speed = controller.heldRun() ? MAX_SPEED * 2.0f : MAX_SPEED;
        const float max_jump = std::abs( vx_ ) >= MAX_SPEED * 1.5f ? JUMP_MAX * 1.1f : JUMP_MAX;

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
        x_ += vx_ * dt;
        if ( x_ != prevx_ )
        {
            NasrGraphicsSpriteSetDestX( graphic_, x_ );
        }
        prevx_ = x_;
        
        if ( isOnGround() )
        {
            if ( !jump_lock_ && controller.heldJump() )
            {
                is_jumping_ = true;
                jump_start_ = y_;
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

        y_ += vy_ * dt;

        if ( isOnGround() )
        {
            y_ = 320.0f - 25.0f;
        }

        if ( y_ != prevy_ )
        {
            NasrGraphicsSpriteSetDestY( graphic_, y_ );
        }
        prevy_ = y_;
        jump_lock_ = controller.heldJump();
    };

    bool Sprite::isOnGround() const
    {
        return y_ >= 320.f - 25.0f;
    };
}