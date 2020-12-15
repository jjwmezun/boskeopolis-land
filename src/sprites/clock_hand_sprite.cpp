#include "camera.hpp"
#include "clock_hand_sprite.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "line.hpp"
#include "test_line_and_box_collision.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"

ClockHandSprite::ClockHandSprite( int x, int y, Direction::Clockwise direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/clock-hand.png" ), x - Unit::BlocksToPixels( 3 ), y - Unit::BlocksToPixels( 3 ), Unit::BlocksToPixels( 8 ), Unit::BlocksToPixels( 8 ), {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
    inner_left_ ( { centerXSubPixels(), centerYSubPixels(), 1, 1 }, centerXSubPixels(), centerYSubPixels(), 4000, (( direction == Direction::Clockwise::COUNTERCLOCKWISE ) ? -0.1 : 0.1 ), -0.333 ),
    inner_right_ ( { centerXSubPixels(), centerYSubPixels(), 1, 1 }, centerXSubPixels(), centerYSubPixels(), 4000, (( direction == Direction::Clockwise::COUNTERCLOCKWISE ) ? -0.1 : 0.1 ), 0.333 ),
    outer_left_ ( { centerXSubPixels(), centerYSubPixels(), 1, 1 }, centerXSubPixels(), centerYSubPixels(), Unit::BlocksToSubPixels( 4 ), (( direction == Direction::Clockwise::COUNTERCLOCKWISE ) ? -0.1 : 0.1 ), -0.0333 ),
    outer_right_ ( { centerXSubPixels(), centerYSubPixels(), 1, 1 }, centerXSubPixels(), centerYSubPixels(), Unit::BlocksToSubPixels( 4 ), (( direction == Direction::Clockwise::COUNTERCLOCKWISE ) ? -0.1 : 0.1 ), 0.0333 ),
    image_box_ ( x + 6, y - 52, 20, 74 ),
    graphics_rotation_amount_ ( ( direction == Direction::Clockwise::COUNTERCLOCKWISE ) ? 93.0 : 83.0 )
{
    graphics_->default_rotation_point_ = true;
    graphics_->rotation_center_ = { 10, 70 };
    graphics_->rotation_ = 85.0;
};

ClockHandSprite::~ClockHandSprite() {};

void ClockHandSprite::customUpdate( LevelState& level_state )
{
    inner_left_.update();
    inner_right_.update();
    outer_left_.update();
    outer_right_.update();
    graphics_->rotation_ = graphics_rotation_amount_ + mezun::convertRadiansToDegrees( outer_right_.getAngle() );
};

void ClockHandSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        const Line left_line {{ inner_left_.position_.x, inner_left_.position_.y }, { outer_left_.position_.x, outer_left_.position_.y }};
        const Line right_line {{ inner_right_.position_.x, inner_right_.position_.y }, { outer_right_.position_.x, outer_right_.position_.y }};
        if
        (
            mezun::testLineAndBoxCollision( left_line, them.hit_box_ ) ||
            mezun::testLineAndBoxCollision( right_line, them.hit_box_ )
        )
        {
            level_state.health().hurt();
        }
    }
};

void ClockHandSprite::render( const Camera& camera ) const
{
    drawHitBox( camera );
    graphics_->render( image_box_, &camera );
    /*
    const Line left_line = camera.relativeLine(Unit::SubPixelsToPixels({{ inner_left_.position_.x, inner_left_.position_.y }, { outer_left_.position_.x, outer_left_.position_.y }}));
    const Line right_line = camera.relativeLine(Unit::SubPixelsToPixels({{ inner_right_.position_.x, inner_right_.position_.y }, { outer_right_.position_.x, outer_right_.position_.y }}));
    Render::renderLine( left_line, 3 );
    Render::renderLine( right_line, 3 );
    */
};
