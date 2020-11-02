#include "devil_fire_sprite.hpp"
#include "devil_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static int generateTimerTarget()
{
    return mezun::randInt( 32, 64 );
};

DevilSprite::DevilSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 32, 32, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 32, 1600, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    timer_ ( 0 ),
    timer_target_ ( generateTimerTarget() )
{};

DevilSprite::~DevilSprite() {};

void DevilSprite::customUpdate( LevelState& level_state )
{
    if ( timer_ >= timer_target_ )
    {
        timer_target_ = generateTimerTarget();
        timer_ = 0;
        const double dx = ( double )( level_state.sprites().hero().centerXSubPixels() - centerXSubPixels() );
        const double dy = ( double )( level_state.sprites().hero().centerYSubPixels() - centerYSubPixels() );
        level_state.sprites().spawn( std::unique_ptr<DevilFireSprite> ( new DevilFireSprite( centerXPixels(), centerYPixels(), dy, dx, std::abs( ( double )( vx_ ) ) ) ) );
    }
    else
    {
        ++timer_;
    }
};

void DevilSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( them.rightSubPixels() < hit_box_.x )
        {
            if ( hit_box_.x < original_hit_box_.x - Unit::BlocksToSubPixels( 10 ) )
            {
                moveRight();
            }
            else
            {
                moveLeft();
            }
        }
        else
        {
            if ( hit_box_.x > original_hit_box_.x + Unit::BlocksToSubPixels( 10 ) )
            {
                moveLeft();
            }
            else
            {
                moveRight();
            }
        }
    }
};
