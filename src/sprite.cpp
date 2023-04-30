#include "config.hpp"
#include "game.hpp"
#include "level.hpp"
#include "map.hpp"
#include "nasringine/nasr.h"
#include "scale_lift_sprite.hpp"
#include "sprite.hpp"
#include "controller.hpp"

#include <iostream>

namespace BSL
{
    static constexpr float START_SPEED = 0.15f;
    static constexpr float MAX_SPEED = 2.0f;
    static constexpr float FALL_SPEED = 0.5f;
    static constexpr float GRAVITY = 6.0f;
    static constexpr float JUMP_ACC = 0.2f;
    static constexpr float JUMP_INIT = 2.0f;
    static constexpr float JUMP_MAX = 4.75f;
    static constexpr float autumn_walk_frames[ 4 ] = { 0.0f, 16.0f, 0.0f, 32.0f };

    Sprite::Sprite
    (
        Type type,
        Rect pos,
        std::set<Attribute> atts,
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
        on_ground_ ( false ),
        dir_x_ ( Dir::X::RIGHT ),
        is_moving_ ( false ),
        animation_timer_ ( 0.0f ),
        start_speed_ ( 0.0f ),
        max_speed_ ( 0.0f ),
        collision_ ( 0.0f, 0.0f, 0.0f, 0.0f ),
        living_ ( LivingState::ALIVE ),
        block_interact_ ( true ),
        max_jump_ ( JUMP_MAX ),
        attributes_ ( atts )
    {
        max_speed_ = GetArg( "maxspeed", args, MAX_SPEED );
        start_speed_ = GetArg( "startspeed", args, START_SPEED );
        dir_x_ = GetArg( "dirx", args, Dir::X::RIGHT );
    };

    void Sprite::init( Game & game )
    {
        switch ( type_ )
        {
            case ( Type::AUTUMN ):
            {
                graphic_ = game.render().addSprite( "sprites/autumn.png", 0.0f, 0.0f, pos_.w, pos_.h, pos_.x, pos_.y );
            }
            break;
            case ( Type::CRAB ):
            {
                graphic_ = game.render().addSprite( "sprites/crab.png", 0.0f, 0.0f, pos_.w, pos_.h, pos_.x, pos_.y );
            }
            break;
            case ( Type::BADAPPLE ):
            {
                graphic_ = game.render().addSprite( "sprites/bad-apple.png", 0.0f, 0.0f, pos_.w, pos_.h, pos_.x, pos_.y );
            }
            break;
            case ( Type::TRUCK ):
            {
                graphic_ = game.render().addSprite( "sprites/grump-truck.png", 0.0f, 0.0f, pos_.w, pos_.h, pos_.x, pos_.y );
            }
            break;
            case ( Type::SCALE_LIFT ):
            {
                initScaleLiftSprite( game, *this );
            }
            break;
        }
    };

    void Sprite::update( float dt, const Controller & controller, Level & level, Game & game, std::vector<Sprite> & sprites )
    {
        switch ( type_ )
        {
            case ( Type::AUTUMN ):
            {
                if ( living_ == LivingState::ALIVE )
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
                        vx_ /= ( 1.0f + 0.5f * dt );
                    }
                    float xchange = vx_ * dt;
                    

                    // Falling & Jumping
                    const bool going_fast = isAutumnGoingFast();
                    const float gravity = controller.heldJump() ? GRAVITY / 1.5f : GRAVITY;
                    const float max_jump = going_fast ? max_jump_ * 1.1f : max_jump_;
                    const bool can_start_jump = misc_.autumn.jump_padding > 0.0f &&
                        !misc_.autumn.jump_lock &&
                        controller.heldJump();

                    // Continue jump.
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
                    // Start jump.
                    else if ( can_start_jump )
                    {
                        is_jumping_ = true;
                        vy_ = -JUMP_INIT;
                        accy_ = -JUMP_ACC;
                    }
                    // Else, fall.
                    else
                    {
                        accy_ = FALL_SPEED;
                    }

                    vy_ += accy_ * dt;
                    if ( vy_ > gravity )
                    {
                        vy_ = gravity;
                    }
                    else if ( vy_ < -max_jump )
                    {
                        max_jump_ = JUMP_MAX;
                        vy_ = -max_jump;
                        is_jumping_ = false;
                        accy_ = 0.0f;
                    }

                    float ychange = vy_ * dt;






                    
                    on_ground_ = false;
                    misc_.autumn.jump_padding = std::max( 0.0f, misc_.autumn.jump_padding - 1.0f * dt);

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
                                autumnLanding();
                            }

                            for ( Sprite & sprite : sprites )
                            {
                                sprite.collideDown( *this, dt, level, game );
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

                            for ( Sprite & sprite : sprites )
                            {
                                sprite.collideUp( *this, dt, level, game );
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
                    // Handle left collision.
                    const int lx = pixelsToBlocks( pos_.x );
                    const bool ltc = map.testCollision( lx, xt );
                    const bool lbc = map.testCollision( lx, xb );
                    if ( ltc || lbc )
                    {
                        pos_.x = blocksToPixels( lx + 1 );
                        vx_ *= -0.25f;
                        accx_ = 0.0f;
                    }

                    for ( Sprite & sprite : sprites )
                    {
                        sprite.collideX( *this, dt, level, game );
                    }


                    // Handle invincibility.
                    if ( misc_.autumn.invincibility_ > 0.0f )
                    {
                        misc_.autumn.invincibility_ = std::max( 0.0f, misc_.autumn.invincibility_ - dt );
                    }




                    // Handle interaction.
                    for ( Sprite & other : sprites )
                    {
                        // Skip self.
                        if ( &other == this ) continue;

                        bool bopped = false;
                        if ( other.hasAttribute( Attribute::BOPPABLE ) )
                        {
                            if
                            (
                                vy_ > 0.0f &&
                                pos_.bottom() > other.pos_.y &&
                                pos_.bottom() < other.pos_.y + 6.0f &&
                                pos_.right() > other.pos_.x &&
                                pos_.x < other.pos_.right()
                            )
                            {
                                bopped = true;
                                other.living_ = LivingState::DYING;
                                is_jumping_ = true;
                                vy_ = -JUMP_INIT;
                                accy_ = -JUMP_ACC;
                                max_jump_ = JUMP_MAX * 1.25f;
                            }
                        }

                        if
                        (
                            other.hasAttribute( Attribute::ENEMY ) &&
                            misc_.autumn.invincibility_ == 0.0f &&
                            !bopped &&
                            pos_.bottom() > other.pos_.y &&
                            pos_.y < other.pos_.bottom() &&
                            pos_.right() > other.pos_.x &&
                            pos_.x < other.pos_.right()
                        )
                        {
                            level.inventory().hurt();
                            misc_.autumn.invincibility_ = 64.0f;
                            if ( level.inventory().isDead() )
                            {
                                living_ = LivingState::DYING;
                                block_interact_ = false;
                            }
                        }
                    }




                    // Graphics
                    // Invincibility
                    graphic_.setOpacity( ( ( int )( misc_.autumn.invincibility_ ) % 4 == 1 ) ? 0.0f : 1.0f );

                    // Rotation
                    rotateOnDirectionChange( dt );


                    if ( !isOnGround() )
                    {
                        graphic_.setSrcX( 48.0f );
                        graphic_.setSrcY( 0.0f );
                    }
                    else
                    {
                        if ( is_moving_ )
                        {
                            if ( ( animation_timer_ += dt ) >= 8.0f )
                            {
                                animation_timer_ -= 8.0f;
                                ++misc_.autumn.walk_frame;
                                if ( misc_.autumn.walk_frame > 3 )
                                {
                                    misc_.autumn.walk_frame = 0;
                                }
                            }
                            graphic_.setSrcX( autumn_walk_frames[ misc_.autumn.walk_frame ] );
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
                    level.setPos( pos_ );

                    // End
                    misc_.autumn.jump_lock = controller.heldJump();
                }
                else
                {
                    vy_ += FALL_SPEED * dt;
                    if ( vy_ > GRAVITY )
                    {
                        vy_ = GRAVITY;
                    }
                    pos_.y += vy_ * dt;

                    updatePositionGraphics();
                }
            }
            break;

            case ( Type::CRAB ):
            {
                handleGeneralCollision( dt, level, game );

                for ( Sprite & other : sprites )
                {
                    // Skip self.
                    if ( &other == this ) continue;

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

                // Graphics
                rotateOnDirectionChange( dt );
                if ( ( animation_timer_ += dt ) >= 8.0f )
                {
                    animation_timer_ -= 8.0f;
                    graphic_.flipX();
                }
                updatePositionGraphics();
            }
            break;

            case ( Type::BADAPPLE ):
            {
                if ( living_ == LivingState::ALIVE )
                {
                    moveInDirectionX();
                    handleGeneralCollision( dt, level, game );
                    flipXOnCollision();

                    // Graphics
                    rotateOnDirectionChange( dt );
                    if ( ( animation_timer_ += dt ) >= 8.0f )
                    {
                        animation_timer_ -= 8.0f;
                        const float current_frame = graphic_.getSrcX();
                        graphic_.setSrcX( current_frame == 0 ? 16.0f : 0.0f );
                    }
                    updatePositionGraphics();
                }
                else
                {
                    block_interact_ = false;
                    handleGeneralCollision( dt, level, game );
                    updatePositionGraphics();
                }
            }
            break;

            case ( Type::TRUCK ):
            {
                moveInDirectionX();
                handleGeneralCollision( dt, level, game );
                flipXOnCollision();

                // Graphics
                rotateOnDirectionChange( dt );
                updatePositionGraphics();
            }
            break;

            case ( Type::SCALE_LIFT ):
            {
                updateScaleLiftSprite( dt, level, sprites, *this );
            }
            break;
        }
    };

    void Sprite::collideDown( Sprite & other, float dt, Level & level, Game & game )
    {
        switch ( type_ )
        {
            case ( Type::TRUCK ):
            {
                if
                (
                    other.pos_.bottom() - 1.0f > pos_.y &&
                    other.pos_.bottom() - 1.0f < pos_.y + 8.0f &&
                    other.pos_.right() > pos_.x &&
                    other.pos_.x < pos_.right()
                )
                {
                    other.collision_.bottom = other.pos_.bottom() - 1.0f - pos_.y;
                    other.pos_.y -= other.collision_.bottom;
                    other.vy_ = 0.0f;
                    other.accy_ = 0.0f;
                    other.autumnLanding();
                    other.pos_.x += ( vx_ * dt );
                    if ( other.is_moving_ )
                    {
                        other.graphic_.setSrcX( autumn_walk_frames[ other.misc_.autumn.walk_frame ] );
                        other.graphic_.setSrcY( 0.0f );
                    }
                    else
                    {
                        other.graphic_.setSrcX( 0.0f );
                        other.graphic_.setSrcY( 0.0f );
                    }
                    other.updatePositionGraphics();
                }
            }
            break;

            case ( Type::SCALE_LIFT ):
            {
                collideDownScaleLiftSprite( *this, other, dt, level, game );
            }
            break;
        }
    };

    void Sprite::collideUp( Sprite & other, float dt, Level & level, Game & game )
    {
        switch ( type_ )
        {
            case ( Type::SCALE_LIFT ):
            {
                collideUpScaleLiftSprite( *this, other, dt, level, game );
            }
            break;
        }
    };

    void Sprite::collideX( Sprite & other, float dt, Level & level, Game & game )
    {
        switch ( type_ )
        {
            case ( Type::SCALE_LIFT ):
            {
                collideXScaleLiftSprite( *this, other, dt, level, game );
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
        collision_ = { 0.0f, 0.0f, 0.0f, 0.0f };
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

    void Sprite::moveInDirectionX()
    {
        switch ( dir_x_ )
        {
            case ( Dir::X::LEFT ):
            {
                goLeft();
            }
            break;
            case ( Dir::X::RIGHT ):
            {
                goRight();
            }
            break;
        }
    };

    void Sprite::handleGeneralCollision( float dt, Level & level, Game & game )
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

        if ( block_interact_ )
        {
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
                        collision_.bottom = pos_.bottom() - 1.0f - blocksToPixels( by );
                        pos_.y -= collision_.bottom;
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
                        collision_.top = 1.0f;
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
                collision_.right = pos_.right() - blocksToPixels( rx );
                pos_.x -= collision_.right;
                vx_ *= -0.25f;
                accx_ = 0.0f;
            }
            // Handle left collision.
            const int lx = pixelsToBlocks( pos_.x );
            const bool ltc = map.testCollision( lx, xt );
            const bool lbc = map.testCollision( lx, xb );
            if ( ltc || lbc )
            {
                collision_.left = 1.0f;
                pos_.x = blocksToPixels( lx + 1 );
                vx_ *= -0.25f;
                accx_ = 0.0f;
            }
        }
        else
        {
            pos_.y += ychange;
            pos_.x += xchange;
        }
    };

    void Sprite::rotateOnDirectionChange( float dt )
    {
        float rotx = graphic_.getRotationX();
        switch ( dir_x_ )
        {
            case ( Dir::X::LEFT ):
            {
                if ( rotx > 0.0f )
                {
                    rotx = std::max( rotx - ( 16.0f * dt ), 0.0f );
                    graphic_.setRotationX( rotx );
                }
            }
            break;
            case ( Dir::X::RIGHT ):
            {
                if ( rotx < 180.0f )
                {
                    rotx = std::min( rotx + ( 16.0f * dt ), 180.0f );
                    graphic_.setRotationX( rotx );
                }
            }
            break;
        }
    };

    void Sprite::flipXOnCollision()
    {
        if ( dir_x_ == Dir::X::LEFT && collision_.left )
        {
            dir_x_ = Dir::X::RIGHT;
        }
        else if ( dir_x_ == Dir::X::RIGHT && collision_.right )
        {
            dir_x_ = Dir::X::LEFT;
        }
    };

    bool Sprite::isAutumnGoingFast() const
    {
        return std::abs( vx_ ) >= MAX_SPEED * 1.5f;
    }

    void Sprite::autumnLanding()
    {
        on_ground_ = true;
        misc_.autumn.jump_padding = isAutumnGoingFast() ? 8.0f : 2.0f;
        max_jump_ = JUMP_MAX;
    };

    bool Sprite::collideSolidDown( const Rect & rect, float dt )
    {
        if
        (
            pos_.right() - 2.0f <= rect.x ||
            pos_.x + 2.0f >= rect.right() ||
            pos_.bottom() - 1.0f <= rect.y ||
            pos_.bottom() - 1.0f >= rect.y + 6.0f
        )
        {
            return false;
        }

        collision_.bottom = pos_.bottom() - 1.0f - rect.y;
        pos_.y -= collision_.bottom;
        vy_ = 0.0f;
        accy_ = 0.0f;
        autumnLanding();

        return true;
    };

    bool Sprite::collideSolidUp( const Rect & rect, float dt )
    {
        if
        (
            pos_.right() - 2.0f <= rect.x ||
            pos_.x + 2.0f >= rect.right() ||
            pos_.y >= rect.bottom() ||
            pos_.y < rect.y
        )
        {
            return false;
        }

        pos_.y = rect.bottom() + 1.0f;
        vy_ *= -0.25f;
        accy_ = 0.0f;
        is_jumping_ = false;

        return true;
    };

    bool Sprite::collideSolidX( const Rect & rect, float dt )
    {
        if
        (
            pos_.y + 2.0f >= rect.bottom() ||
            pos_.bottom() - 3.0f <= rect.y ||
            pos_.right() <= rect.x ||
            pos_.x >= rect.right()
        )
        {
            return false;
        }

        if ( pos_.centerX() > rect.centerX() )
        {
            pos_.x += rect.right() - pos_.x + 1.0f;
        }
        else
        {
            pos_.x -= pos_.right() - rect.x + 1.0f;
        }
        vx_ = 0.0f;
        accx_ = 0.0f;

        return true;
    };
}