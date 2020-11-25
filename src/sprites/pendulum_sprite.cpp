#include "pendulum_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

#include <cstdio>

PendulumSprite::PendulumSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y - 128, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    movement_( hit_box_, hit_box_.x, hit_box_.y, 96000, 0.025 ),
    keep_player_ ( false ),
    prev_x_ ( 0 ),
    keep_player_x_ ( false )
{};

PendulumSprite::~PendulumSprite() {};

void PendulumSprite::customUpdate( LevelState& level_state )
{
    prev_x_ = hit_box_.x;
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::LEFT ):
        {
            movement_.updateAngleClockwise();
            if ( movement_.getAngle() >= 2.4 )
            {
                direction_x_ = Direction::Horizontal::RIGHT;
                keep_player_x_ = false;
            }
        }
        break;
        case ( Direction::Horizontal::RIGHT ):
        {
            movement_.updateAngleCounterClockwise();
            if ( movement_.getAngle() <= 0.8 )
            {
                direction_x_ = Direction::Horizontal::LEFT;
                keep_player_x_ = false;
            }
        }
        break;
    }
    movement_.updatePosition();
    hit_box_ = movement_.getPosition();
};

void PendulumSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
        if ( keep_player_ && !them.jump_start_ )
        {
            them.hit_box_.y = hit_box_.y - them.hit_box_.h + 1000;
        }
        if ( keep_player_x_ )
        {
            them.hit_box_.x -= prev_x_ - hit_box_.x;
        }
        keep_player_ = false;
	    Collision collision = them.testBlockCollision( *this );
        if ( collision.collideAny() )
        {
            them.collideStopAny( collision );
            if ( collision.collideBottom() && !them.jump_start_ )
            {
                keep_player_ = true;
                keep_player_x_ = true;
            }
        }
        if ( them.on_ground_ && !collision.collideBottom() )
        {
            keep_player_x_ = false;
        }
	}
};
