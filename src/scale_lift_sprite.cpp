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
    static constexpr float WEIGHT_SHOW_SCORE_TIMER_LIMIT = 40.0f;
    static constexpr float WEIGHT_SHOW_SCORE_TIMER_RISING_LIMIT = WEIGHT_SHOW_SCORE_TIMER_LIMIT - 10.0f;

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
        Rect & left = sprite.misc_.scale_lift.left;
        Rect & right = sprite.misc_.scale_lift.right;
        left.x = sprite.pos_.x;
        left.y = sprite.dir_x_ == Dir::X::LEFT ? y - blocksToPixels( 2.0f ) : y + blocksToPixels( 2.0f );
        right.x = sprite.pos_.x + WEIGHT_WIDTH + WEIGHT_SPACE_APART;
        right.y = sprite.dir_x_ == Dir::X::RIGHT ? y - blocksToPixels( 2.0f ) : y + blocksToPixels( 2.0f );
        left.w = right.w = WEIGHT_WIDTH;
        left.h = right.h = WEIGHT_HEIGHT;
        sprite.misc_.scale_lift.top_bar_gfx = game.render().addSprite( "sprites/scale-lifts.png", 0.0f, WEIGHT_HEIGHT, WEIGHT_WIDTH + WEIGHT_SPACE_APART, 4.0f, sprite.pos_.x + WEIGHT_LEFT_BAR_X + 1.0f, wheel_y );
        sprite.misc_.scale_lift.left_bar_gfx = game.render().addSprite( "sprites/scale-lifts.png", 288.0f, 0.0f, 4.0f, left.y - sprite.pos_.y + 10.0f, sprite.pos_.x + WEIGHT_LEFT_BAR_X, wheel_y + 1.0f );
        sprite.misc_.scale_lift.right_bar_gfx = game.render().addSprite( "sprites/scale-lifts.png", 288.0f, 0.0f, 4.0f, right.y - sprite.pos_.y + 10.0f, sprite.pos_.x + WEIGHT_RIGHT_BAR_X, wheel_y + 1.0f );
        sprite.misc_.scale_lift.left_wheel_gfx = game.render().addSprite( "sprites/scale-lifts.png", 48.0f, 0.0f, 8.0f, 8.0f, sprite.pos_.x + WEIGHT_LEFT_BAR_X - 2.0f, wheel_y - 2.0f );
        sprite.misc_.scale_lift.right_wheel_gfx = game.render().addSprite( "sprites/scale-lifts.png", 48.0f, 0.0f, 8.0f, 8.0f, sprite.pos_.x + WEIGHT_RIGHT_BAR_X - 2.0f, wheel_y - 2.0f );
        sprite.misc_.scale_lift.left_gfx = game.render().addSprite( "sprites/scale-lifts.png", 0.0f, 0.0f, left.w, left.h, left.x, left.y );
        sprite.misc_.scale_lift.right_gfx = game.render().addSprite( "sprites/scale-lifts.png", 0.0f, 0.0f, right.w, right.h, right.x, right.y );
    }

    void updateScaleLiftSprite( float dt, Level & level, std::vector<Sprite> & sprites, Sprite & sprite )
    {
        // Handle interaction.
        for ( Sprite & other : sprites )
        {
            // Skip if not autumn.
            if ( other.type_ != Sprite::Type::AUTUMN ) continue;

            const Rect & left = sprite.misc_.scale_lift.left;

            if
            (
                other.pos_.right() - 2.0f > left.x &&
                other.pos_.x + 2.0f < left.right()
            )
            {
                if
                (
                    other.pos_.bottom() - 1.0f > left.y &&
                    other.pos_.bottom() - 1.0f < left.y + 6.0f
                )
                {
                    other.collision_.bottom = other.pos_.bottom() - 1.0f - left.y;
                    other.pos_.y -= other.collision_.bottom;
                    other.vy_ = 0.0f;
                    other.accy_ = 0.0f;
                    other.autumnLanding();
                    if ( other.is_moving_ )
                    {
                        //other.graphic_.setSrcX( autumn_walk_frames[ other.misc_.autumn.walk_frame ] );
                        other.graphic_.setSrcY( 0.0f );
                    }
                    else
                    {
                        other.graphic_.setSrcX( 0.0f );
                        other.graphic_.setSrcY( 0.0f );
                    }
                    other.updatePositionGraphics();
                }
                else if
                (
                    other.pos_.y < left.bottom() &&
                    other.pos_.y >= left.y
                )
                {
                    other.pos_.y = left.bottom() + 1.0f;
                    other.vy_ *= -0.25f;
                    other.accy_ = 0.0f;
                    other.is_jumping_ = false;
                }
            }

            if
            (
                other.pos_.y + 2.0f < left.bottom() &&
                other.pos_.bottom() - 3.0f > left.y
            )
            {
                if
                (
                    other.pos_.right() > left.x &&
                    other.pos_.x < left.right()
                )
                {
                    if ( other.pos_.centerX() > left.centerX() )
                    {
                        other.pos_.x += left.right() - other.pos_.x + 1.0f;
                    }
                    else
                    {
                        other.pos_.x -= other.pos_.right() - left.x + 1.0f;
                    }
                    other.vx_ = 0.0f;
                    other.accx_ = 0.0f;
                    //other.updatePositionGraphics();
                }
            }
        }
    };
};