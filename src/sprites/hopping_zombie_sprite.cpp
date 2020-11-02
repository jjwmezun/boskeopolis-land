#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "hopping_zombie_sprite.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

static int generateRandomTargetTime()
{
    return mezun::randInt( 8, 1 );
};

static int generateRandomJumpHeight()
{
    return ( mezun::randBool() ) ? 4000 : 6000;
};

HoppingZombieSprite::HoppingZombieSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/zombie.png", 0, 0, false, false, 0.0, -2, -1, 5, 2 ), x, y, 10, 26, {}, 100, 1000, 1000, generateRandomJumpHeight(), Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    timer_ ( 0 ),
    target_time_ ( generateRandomTargetTime() )
{
    jump_lock_ = false;
};

HoppingZombieSprite::~HoppingZombieSprite() {};

void HoppingZombieSprite::customUpdate( LevelState& level_state )
{
    if ( jump_start_ || is_jumping_ )
    {
        jump();
        moveInDirectionX();
        turnOnCollide();
    }
    else if ( on_ground_ )
    {
        fullStopX();
        if ( timer_ == target_time_ )
        {
            target_time_ = generateRandomTargetTime();
            timer_ = 0;
            jump_top_speed_ = generateRandomJumpHeight();
            jump();
        }
        else
        {
            ++timer_;
        }
    }
};

void HoppingZombieSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( their_collision.collideBottom() && them.vy_ > 0 )
        {
            kill();
            them.bounce();
            if ( them.movementType() != SpriteMovement::Type::SWIMMING )
            {
                level_state.inventory().bop();
            }
            Audio::playSound( Audio::SoundType::BOP );
        }
        else if ( their_collision.collideAny() && level_state.events().testIsSlidingPreviously() )
        {
            kill();
            Audio::playSound( Audio::SoundType::BOP );
        }
        else if ( !isDead() && their_collision.collideAny() )
        {
            level_state.health().hurt();
        }
    }
};
