#include "config.hpp"
#include "game.hpp"
#include "level.hpp"
#include "map.hpp"
#include "nasringine/nasr.h"
#include "sprite.hpp"
#include "controller.hpp"

#include <iostream>

constexpr float START_SPEED = 0.15f;
constexpr float MAX_SPEED = 2.0f;
constexpr float FALL_SPEED = 0.5f;
constexpr float GRAVITY = 6.0f;
constexpr float JUMP_ACC = 0.2f;
constexpr float JUMP_INIT = 2.0f;
constexpr float JUMP_MAX = 4.75f;

namespace BSL
{
    Sprite::Sprite
    (
        Type type,
        Rect pos,
        ArgList args
    ) :
        type_ ( type ),
        pos_ ( pos ),
        accx_ ( 0.0f ),
        vx_ ( 0.0f ),
        prevx_ ( 0.0f ),
        accy_ ( 0.0f ),
        vy_ ( 0.0f ),
        prevy_ ( 0.0f ),
        is_jumping_ ( false ),
        jump_lock_ ( false ),
        on_ground_ ( false ),
        jump_padding_ ( 0.0f ),
        dir_x_ ( Dir::X::RIGHT ),
        is_moving_ ( false ),
        animation_timer_ ( 0.0f ),
        start_speed_ ( 0.0f ),
        max_speed_ ( 0.0f )
    {
        max_speed_ = GetArg( "maxspeed", args, MAX_SPEED );
        start_speed_ = GetArg( "startspeed", args, START_SPEED );
    };

    void Sprite::init( Game & game )
    {
        switch ( type_ )
        {
            case ( Type::AUTUMN ):
            {
                graphic_ = game.render().addSprite( "sprites/autumn.png", 0.0f, 0.0f, pos_.w, pos_.h, pos_.x, pos_.y );
                attributes_ = {
                    Attribute::PROTAG
                };
            }
            break;
            case ( Type::CRAB ):
            {
                graphic_ = game.render().addSprite( "sprites/crab.png", 0.0f, 0.0f, pos_.w, pos_.h, pos_.x, pos_.y );
                attributes_ = {
                    Attribute::ENEMY
                };
            }
            break;
        }
    };

    void Sprite::update( float dt, const Controller & controller, Level & level, Game & game )
    {
        switch ( type_ )
        {
            case ( Type::AUTUMN ):
            {
                // Horizontal Movement
                start_speed_ = controller.heldRun() ? START_SPEED * 2.0f : START_SPEED;
                max_speed_ = controller.heldRun() ? MAX_SPEED * 2.0f : MAX_SPEED;
                if ( controller.heldRight() )
                {
                    goRight();
                }
                else if ( controller.heldLeft() )
                {
                    goLeft();
                }
                else
                {
                    accx_ = 0.0f;
                    is_moving_ = false;
                }

                vx_ += accx_ * dt;
                if ( vx_ > max_speed_ )
                {
                    vx_ = max_speed_;
                }
                else if ( vx_ < -max_speed_ )
                {
                    vx_ = -max_speed_;
                }
                if ( accx_ == 0.0f )
                {
                    vx_ /= ( 1.0f + 0.2f * dt );
                }
                float xchange = vx_ * dt;
                

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

                float ychange = vy_ * dt;






                
                on_ground_ = false;
                jump_padding_ = std::max( 0.0f, jump_padding_ - 1.0f * dt);

                // Collision
                Map & map = level.map();

                // General block collision.
                map.interact( *this, level, game );

                // Handle downward collision.
                if ( ychange > 0.0f )
                {
                    const float ystep = 4.0f;
                    while ( ychange != 0.0f )
                    {
                        pos_.y += std::min( ystep, ychange );
                        ychange = std::max( 0.0f, ychange - ystep );

                        const int bl = pixelsToBlocks( pos_.x + 2.0f );
                        const int br = pixelsToBlocks( pos_.right() - 2.0f );
                        const int by = pixelsToBlocks( pos_.bottom() - 1.0f );
                        const std::vector<Map::CollisionType> ctypes { Map::CollisionType::SOLID_ABOVE };
                        const bool blc = map.testCollision( bl, by );
                        const bool brc = map.testCollision( br, by );
                        const bool blc2 = map.testCollision( bl, by, ctypes );
                        const bool brc2 = map.testCollision( br, by, ctypes );
                        const float bydiff = pos_.bottom() - 1.0f - static_cast<float>( blocksToPixels( by ) );
                        const bool flat_top_coll = ( blc2 || brc2 ) && bydiff <= 4.0f;
                        if ( blc || brc || flat_top_coll )
                        {
                            pos_.y -= pos_.bottom() - 1.0f - blocksToPixels( by );
                            on_ground_ = true;
                            jump_padding_ = going_fast ? 8.0f : 2.0f;
                        }
                    }
                }
                // Handle upward collision.
                else if ( ychange < 0.0f )
                {
                    const float ystep = -12.0f;
                    while ( ychange != 0.0f )
                    {
                        pos_.y += std::max( ystep, ychange );
                        ychange = std::min( 0.0f, ychange - ystep );

                        const int tl = pixelsToBlocks( pos_.x + 4.0f );
                        const int tr = pixelsToBlocks( pos_.right() - 4.0f );
                        const int ty = pixelsToBlocks( pos_.y );
                        const bool tlc = map.testCollision( tl, ty );
                        const bool trc = map.testCollision( tr, ty );
                        if ( tlc || trc )
                        {
                            pos_.y = blocksToPixels( ty + 1 );
                            vy_ *= -0.25f;
                            accy_ = 0.0f;
                            is_jumping_ = false;
                        }
                    }
                }

                pos_.x += xchange;

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
                float rotx = graphic_.getRotationX();
                switch ( dir_x_ )
                {
                    case ( Dir::X::LEFT ):
                    {
                        if ( rotx > 0.0f )
                        {
                            rotx = std::max( rotx - ( 4.0f * dt ), 0.0f );
                            graphic_.setRotationX( rotx );
                        }
                    }
                    break;
                    case ( Dir::X::RIGHT ):
                    {
                        if ( rotx < 180.0f )
                        {
                            rotx = std::min( rotx + ( 4.0f * dt ), 180.0f );
                            graphic_.setRotationX( rotx );
                        }
                    }
                    break;
                }
                if ( !isOnGround() )
                {
                    graphic_.setSrcX( 48.0f );
                    graphic_.setSrcY( 0.0f );
                }
                else
                {
                    if ( is_moving_ )
                    {
                        static float frames[ 4 ] = { 0.0f, 16.0f, 0.0f, 32.0f };
                        if ( ( animation_timer_ += dt ) >= 8.0f )
                        {
                            animation_timer_ -= 8.0f;
                            ++walk_frame_;
                        }
                        graphic_.setSrcX( frames[ walk_frame_.get() ] );
                        graphic_.setSrcY( 0.0f );
                    }
                    else
                    {
                        graphic_.setSrcX( 0.0f );
                        graphic_.setSrcY( 0.0f );
                    }
                }

                updatePositionGraphics();

                NasrRect r = graphic_.getDest();
                NasrAdjustCamera( &r, map.getWidthPixels(), map.getHeightPixels() );

                // End
                jump_lock_ = controller.heldJump();
            }
            break;

            case ( Type::CRAB ):
            {
                vx_ += accx_ * dt;
                if ( vx_ > max_speed_ )
                {
                    vx_ = max_speed_;
                }
                else if ( vx_ < -max_speed_ )
                {
                    vx_ = -max_speed_;
                }
                if ( accx_ == 0.0f )
                {
                    vx_ /= ( 1.0f + 0.2f * dt );
                }
                float xchange = vx_ * dt;
                

                // Falling & Jumping
                const float gravity = GRAVITY;
                accy_ = FALL_SPEED;
                vy_ += accy_ * dt;
                if ( vy_ > gravity )
                {
                    vy_ = gravity;
                }
                float ychange = vy_ * dt;






                
                on_ground_ = false;

                // Collision
                Map & map = level.map();

                // General block collision.
                map.interact( *this, level, game );

                // Handle downward collision.
                if ( ychange > 0.0f )
                {
                    const float ystep = 4.0f;
                    while ( ychange != 0.0f )
                    {
                        pos_.y += std::min( ystep, ychange );
                        ychange = std::max( 0.0f, ychange - ystep );

                        const int bl = pixelsToBlocks( pos_.x + 2.0f );
                        const int br = pixelsToBlocks( pos_.right() - 2.0f );
                        const int by = pixelsToBlocks( pos_.bottom() - 1.0f );
                        const std::vector<Map::CollisionType> ctypes { Map::CollisionType::SOLID_ABOVE };
                        const bool blc = map.testCollision( bl, by );
                        const bool brc = map.testCollision( br, by );
                        const bool blc2 = map.testCollision( bl, by, ctypes );
                        const bool brc2 = map.testCollision( br, by, ctypes );
                        const float bydiff = pos_.bottom() - 1.0f - static_cast<float>( blocksToPixels( by ) );
                        const bool flat_top_coll = ( blc2 || brc2 ) && bydiff <= 4.0f;
                        if ( blc || brc || flat_top_coll )
                        {
                            pos_.y -= pos_.bottom() - 1.0f - blocksToPixels( by );
                            on_ground_ = true;
                        }
                    }
                }
                // Handle upward collision.
                else if ( ychange < 0.0f )
                {
                    const float ystep = -12.0f;
                    while ( ychange != 0.0f )
                    {
                        pos_.y += std::max( ystep, ychange );
                        ychange = std::min( 0.0f, ychange - ystep );

                        const int tl = pixelsToBlocks( pos_.x + 4.0f );
                        const int tr = pixelsToBlocks( pos_.right() - 4.0f );
                        const int ty = pixelsToBlocks( pos_.y );
                        const bool tlc = map.testCollision( tl, ty );
                        const bool trc = map.testCollision( tr, ty );
                        if ( tlc || trc )
                        {
                            pos_.y = blocksToPixels( ty + 1 );
                            vy_ *= -0.25f;
                            accy_ = 0.0f;
                        }
                    }
                }

                pos_.x += xchange;

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
                float rotx = graphic_.getRotationX();
                switch ( dir_x_ )
                {
                    case ( Dir::X::LEFT ):
                    {
                        if ( rotx > 0.0f )
                        {
                            rotx = std::max( rotx - ( 4.0f * dt ), 0.0f );
                            graphic_.setRotationX( rotx );
                        }
                    }
                    break;
                    case ( Dir::X::RIGHT ):
                    {
                        if ( rotx < 180.0f )
                        {
                            rotx = std::min( rotx + ( 4.0f * dt ), 180.0f );
                            graphic_.setRotationX( rotx );
                        }
                    }
                    break;
                }
                if ( ( animation_timer_ += dt ) >= 8.0f )
                {
                    animation_timer_ -= 8.0f;
                    graphic_.flipX();
                }
                updatePositionGraphics();
            }
            break;
        }
    };

    void Sprite::interact( Sprite & other, Level & level, Game & game )
    {
        switch ( type_ )
        {
            case ( Type::CRAB ):
            {
                if ( other.hasAttribute( Attribute::PROTAG ) )
                {
                    if ( other.pos_.right() < pos_.x )
                    {
                        goLeft();
                    }
                    else
                    {
                        goRight();
                    }
                }
            }
            break;
        }
    };

    bool Sprite::isOnGround() const
    {
        return on_ground_;
    };

    void Sprite::updatePositionGraphics()
    {
        if ( pos_.x != prevx_ )
        {
            graphic_.setDestX( pos_.x );
        }
        if ( pos_.y != prevy_ )
        {
            graphic_.setDestY( pos_.y );
        }
        prevx_ = pos_.x;
        prevy_ = pos_.y;
    };

    void Sprite::goLeft()
    {
        dir_x_ = Dir::X::LEFT;
        accx_ = -start_speed_;
        is_moving_ = true;
    };

    void Sprite::goRight()
    {
        dir_x_ = Dir::X::RIGHT;
        accx_ = start_speed_;
        is_moving_ = true;
    };

    bool Sprite::hasAttribute( Attribute att ) const
    {
        return attributes_.find( att ) != attributes_.end();
    };
}