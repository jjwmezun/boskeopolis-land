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
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/zombie.png", 0, 0, false, false, 0.0, 0, 0, 0, 0 ), x, y, 18, 31, {}, 100, 1000, 1000, generateRandomJumpHeight(), Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    timer_ ( 0 ),
    target_time_ ( generateRandomTargetTime() ),
	head_box_ ( Unit::PixelsToSubPixels( sdl2::SDLRect{ x + 2, y + 3, 16, 13 } )),
	hurt_box_ ( Unit::PixelsToSubPixels( sdl2::SDLRect{ x + 5, y + 17, 9, 13 } ))
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
        graphics_->current_frame_x_ = 36;
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
        graphics_->current_frame_x_ = 0;
    }

    head_box_.x = hit_box_.x + Unit::PixelsToSubPixels( 2 );
    head_box_.y = hit_box_.y + Unit::PixelsToSubPixels( 3 );
    hurt_box_.x = hit_box_.x + Unit::PixelsToSubPixels( 5 );
    hurt_box_.y = hit_box_.y + Unit::PixelsToSubPixels( 17 );
    flipGraphicsOnRight();
};

void HoppingZombieSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
		const Collision head_collision = them.testCollision( head_box_ );
        if ( !them.on_ground_ && head_collision.collideAny() )
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
        else
        {
            const Collision hurt_collision = them.testCollision( hurt_box_ );
            if ( hurt_collision.collideAny() )
            {
                level_state.health().hurt();
            }
        }
    }
};
