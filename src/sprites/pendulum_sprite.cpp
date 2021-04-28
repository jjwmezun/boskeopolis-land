#include "pendulum_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

#include <cstdio>

static constexpr double LEFTMOST_ANGLE = 2.4;
static constexpr double RIGHTMOST_ANGLE = 0.8;

PendulumSprite::PendulumSprite( int x, int y, Direction::Horizontal start )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x - 54, y - 49, 168, 65, {}, 0, 0, 0, 0, start, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    movement_( { Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y - 128 ), Unit::PixelsToSubPixels( 32 ), Unit::PixelsToSubPixels( 32 ) }, Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y - 128 ), Unit::PixelsToSubPixels( 96 ), 0.025, ( start == Direction::Horizontal::RIGHT ) ? LEFTMOST_ANGLE : RIGHTMOST_ANGLE ),
    keep_player_ ( false ),
    prev_x_ ( 0 ),
    keep_player_x_ ( false ),
    keep_player_x2_ ( false ),
    solid_ball_ ( Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y - 128 ), Unit::PixelsToSubPixels( 32 ), Unit::PixelsToSubPixels( 32 ) ),
    max_ ( -99999 ),
    min_ ( 99999 )
{};

PendulumSprite::~PendulumSprite() {};

void PendulumSprite::customUpdate( LevelState& level_state )
{
    prev_x_ = solid_ball_.x;
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::LEFT ):
        {
            movement_.updateAngleClockwise();
            if ( movement_.getAngle() >= LEFTMOST_ANGLE )
            {
                movement_.angle_ = LEFTMOST_ANGLE;
                direction_x_ = Direction::Horizontal::RIGHT;
            }
        }
        break;
        case ( Direction::Horizontal::RIGHT ):
        {
            movement_.updateAngleCounterClockwise();
            if ( movement_.getAngle() <= RIGHTMOST_ANGLE )
            {
                movement_.angle_ = RIGHTMOST_ANGLE;
                direction_x_ = Direction::Horizontal::LEFT;
            }
        }
        break;
    }

    movement_.updatePosition();
    solid_ball_ = movement_.getPosition();
};

void PendulumSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
        if ( keep_player_ && !them.jump_start_ )
        {
            them.hit_box_.y = solid_ball_.y - them.hit_box_.h + 1000;
        }
        if ( keep_player_x_ )
        {
            them.hit_box_.x -= prev_x_ - solid_ball_.x;
        }
        keep_player_ = false;
	    Collision collision = them.testBlockCollision( solid_ball_ );
        if ( collision.collideAny() )
        {
            them.collideStopAny( collision );
            if ( collision.collideBottom() && !them.jump_start_ )
            {
                keep_player_ = true;
                keep_player_x_ = true;
                //keep_player_x2_ = true;
            }
        }
        if ( !collision.collideBottom() )
        {
            keep_player_x_ = false;
        }
	}
};

void PendulumSprite::render( const Camera& camera ) const
{
    drawHitBox( camera );
    graphics_->render( Unit::SubPixelsToPixels( solid_ball_ ), &camera );
};
