#include "config.hpp"
#include "game.hpp"
#include "rect.hpp"
#include "scale_lift_sprite.hpp"

namespace BSL
{
    static constexpr float WEIGHT_WIDTH = 48.0f;
    static constexpr float WEIGHT_HEIGHT = 8.0f;
    static constexpr float WEIGHT_SPACE_APART = 80.0f;
    static constexpr float WEIGHT_LEFT_BAR_X = WEIGHT_WIDTH / 2.0f - 2.0f;
    static constexpr float WEIGHT_RIGHT_BAR_X = WEIGHT_LEFT_BAR_X + WEIGHT_WIDTH + WEIGHT_SPACE_APART;
    static constexpr float WEIGHT_TOTAL_WIDTH = WEIGHT_WIDTH * 2.0f + WEIGHT_SPACE_APART;
    static constexpr float WEIGHT_DIGITS_IN_SCORE = 3.0f;
    static constexpr float WEIGHT_BREAK_LIMIT = 15.0f;
    static constexpr float WEIGHT_FALL_START = 100.0f;
    static constexpr float WEIGHT_ACC = 0.025f;
    static constexpr float WEIGHT_TOP_SPEED = 2.0f;
    static constexpr float WEIGHT_FALL_SPEED = 3.0f;

    #define LIFT sprite.misc_.scale_lift

    static constexpr bool isScaleLiftBroken( Sprite & sprite )
    {
        return LIFT.broken_timer >= WEIGHT_BREAK_LIMIT;
    };

    static constexpr bool isScaleLiftFalling( Sprite & sprite )
    {
        return LIFT.broken_timer >= WEIGHT_FALL_START;
    };

    static void makeScaleLiftFall( Sprite & sprite )
    {
        LIFT.broken_timer = WEIGHT_FALL_START;
    };

    Sprite createScaleLiftSprite( float x, float y, Dir::X dir )
    {
        return Sprite
        (
            Sprite::Type::SCALE_LIFT,
            Rect{ blocksToPixels( x ), blocksToPixels( y - 4.0f ), blocksToPixels( WEIGHT_TOTAL_WIDTH ), blocksToPixels( 8.0f ) }
        );
    }

    void initScaleLiftSprite( Game & game, Sprite & sprite )
    {
        const float y = sprite.pos_.y + blocksToPixels( 4.0f );
        const float wheel_y = sprite.pos_.y - 10.f;
        Rect & left = LIFT.left;
        Rect & right = LIFT.right;
        left.x = sprite.pos_.x;
        left.y = sprite.dir_x_ == Dir::X::LEFT ? y - blocksToPixels( 2.0f ) : y + blocksToPixels( 2.0f );
        right.x = sprite.pos_.x + WEIGHT_WIDTH + WEIGHT_SPACE_APART;
        right.y = sprite.dir_x_ == Dir::X::RIGHT ? y - blocksToPixels( 2.0f ) : y + blocksToPixels( 2.0f );
        left.w = right.w = WEIGHT_WIDTH;
        left.h = right.h = WEIGHT_HEIGHT;
        LIFT.wheel_rotation = 0.0f;
        LIFT.top_bar_gfx = game.render().addSprite( "sprites/scale-lifts.png", 0.0f, WEIGHT_HEIGHT, WEIGHT_WIDTH + WEIGHT_SPACE_APART, 4.0f, sprite.pos_.x + WEIGHT_LEFT_BAR_X + 1.0f, wheel_y );
        LIFT.left_bar_gfx = game.render().addSprite( "sprites/scale-lifts.png", 288.0f, 0.0f, 4.0f, left.y - sprite.pos_.y + 10.0f, sprite.pos_.x + WEIGHT_LEFT_BAR_X, wheel_y + 1.0f );
        LIFT.right_bar_gfx = game.render().addSprite( "sprites/scale-lifts.png", 288.0f, 0.0f, 4.0f, right.y - sprite.pos_.y + 10.0f, sprite.pos_.x + WEIGHT_RIGHT_BAR_X, wheel_y + 1.0f );
        LIFT.left_wheel_gfx = game.render().addSprite( "sprites/scale-lifts.png", 48.0f, 0.0f, 8.0f, 8.0f, sprite.pos_.x + WEIGHT_LEFT_BAR_X - 2.0f, wheel_y - 2.0f );
        LIFT.right_wheel_gfx = game.render().addSprite( "sprites/scale-lifts.png", 48.0f, 0.0f, 8.0f, 8.0f, sprite.pos_.x + WEIGHT_RIGHT_BAR_X - 2.0f, wheel_y - 2.0f );
        LIFT.left_gfx = game.render().addSprite( "sprites/scale-lifts.png", 0.0f, 0.0f, left.w, left.h, left.x, left.y );
        LIFT.right_gfx = game.render().addSprite( "sprites/scale-lifts.png", 0.0f, 0.0f, right.w, right.h, right.x, right.y );
        LIFT.neither_is_pressed_on = true;
    }

    static void keepPlatformWithinLimits( Sprite & sprite, Rect & platform )
    {
        if ( platform.y > sprite.pos_.bottom() )
        {
            platform.y = sprite.pos_.bottom();
            sprite.accy_ = 0.0f;
            sprite.vy_ = 0.0f;
        }
        else if ( platform.y < sprite.pos_.y )
        {
            platform.y = sprite.pos_.y;
            sprite.accy_ = 0.0f;
            sprite.vy_ = 0.0f;
        }
    };

    void updateScaleLiftSprite( float dt, Level & level, std::vector<Sprite> & sprites, Sprite & sprite )
    {
        if ( isScaleLiftBroken( sprite ) )
        {
            if ( isScaleLiftFalling( sprite ) )
            {
                LIFT.left.y  += WEIGHT_FALL_SPEED;
                LIFT.right.y += WEIGHT_FALL_SPEED;
            }
            else
            {
                makeScaleLiftFall( sprite );
            }
        }
        else
        {
            if
            (
                LIFT.neither_is_pressed_on
            )
            {
                sprite.accy_ = 0.0f;
                sprite.vy_ /= 1.1f;
            }
            else
            {
                sprite.vy_ += sprite.accy_;
                if ( sprite.vy_ >= WEIGHT_TOP_SPEED )
                {
                    sprite.vy_ = WEIGHT_TOP_SPEED;
                }
                else if ( sprite.vy_ <= -WEIGHT_TOP_SPEED )
                {
                    sprite.vy_ = -WEIGHT_TOP_SPEED;
                }
            }

            LIFT.left.y += sprite.vy_;
            LIFT.right.y -= sprite.vy_;

            keepPlatformWithinLimits( sprite, LIFT.left );
            keepPlatformWithinLimits( sprite, LIFT.right );

            LIFT.left_bar_gfx.setDestH( LIFT.left.y - sprite.pos_.y + 10.0f );
            LIFT.right_bar_gfx.setDestH( LIFT.right.y - sprite.pos_.y + 10.0f );
            LIFT.left_bar_gfx.setSrcH( LIFT.left.y - sprite.pos_.y + 10.0f );
            LIFT.right_bar_gfx.setSrcH( LIFT.right.y - sprite.pos_.y + 10.0f );
            LIFT.left_wheel_gfx.setRotationY( LIFT.wheel_rotation );
            LIFT.right_wheel_gfx.setRotationY( LIFT.wheel_rotation );
            const float lh = std::min( sprite.pos_.h, LIFT.left.y - sprite.pos_.y ) + 9.0f;
            const float rh = std::min( sprite.pos_.h, LIFT.right.y - sprite.pos_.y ) + 9.0f;
            LIFT.top_bar_gfx.setSrcX( lh );
            LIFT.left_bar_gfx.setSrcY( 192.0f - lh );
            LIFT.right_bar_gfx.setSrcY( 192.0f - rh );
        }

        LIFT.left_gfx.setDest( LIFT.left );
        LIFT.right_gfx.setDest( LIFT.right );

        LIFT.neither_is_pressed_on = true;
    };

    void collideDownScaleLiftSprite( Sprite & sprite, Sprite & other, float dt, Level & level, Game & game )
    {
        if ( !isScaleLiftBroken( sprite ) )
        {
            bool any_at_bottom = false;
            if
            (
                other.collideSolidDown( LIFT.left, dt )
            )
            {
                sprite.accy_ = WEIGHT_ACC;
                LIFT.neither_is_pressed_on = false;
                LIFT.wheel_rotation -= std::abs( sprite.vy_ ) * 10.0f;

                if ( other.hasAttribute( Sprite::Attribute::PROTAG ) && LIFT.left.y >= sprite.pos_.bottom() )
                {
                    any_at_bottom = true;
                    LIFT.broken_timer += dt;
                }
            }

            if
            (
                other.collideSolidDown( LIFT.right, dt )
            )
            {
                sprite.accy_ = -WEIGHT_ACC;
                LIFT.neither_is_pressed_on = false;
                LIFT.wheel_rotation += std::abs( sprite.vy_ ) * 10.0f;

                if ( other.hasAttribute( Sprite::Attribute::PROTAG ) && LIFT.right.y >= sprite.pos_.bottom() )
                {
                    any_at_bottom = true;
                    LIFT.broken_timer += dt;
                }
            }

            if ( !any_at_bottom )
            {
                LIFT.broken_timer = 0.0f;
            }
        }
    };

    void collideUpScaleLiftSprite( Sprite & sprite, Sprite & other, float dt, Level & level, Game & game )
    {
        other.collideSolidUp( LIFT.left, dt );
        other.collideSolidUp( LIFT.right, dt );
    };

    void collideXScaleLiftSprite( Sprite & sprite, Sprite & other, float dt, Level & level, Game & game )
    {
        other.collideSolidX( LIFT.left, dt );
        other.collideSolidX( LIFT.right, dt );
    };

    #undef LIFT
};