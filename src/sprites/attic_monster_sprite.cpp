#include "attic_monster_sprite.hpp"
#include "attic_monster_projectile_sprite.hpp"
#include "audio.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "inventory.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

AtticMonsterSprite::AtticMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y + 20, 24, 24, {}, 100, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
    timer_ ( 0 ),
    state_ ( State::DORMANT )
{};

AtticMonsterSprite::~AtticMonsterSprite() {};

void AtticMonsterSprite::customUpdate( LevelState& level_state )
{
    switch ( state_ )
    {
        case ( State::DORMANT ):
        {
        }
        break;
        case ( State::RISING ):
        {
            moveUp();
            if ( hit_box_.y <= original_hit_box_.y - Unit::PixelsToSubPixels( 24 ) )
            {
                fullStopY();
                hit_box_.y = original_hit_box_.y - Unit::PixelsToSubPixels( 24 );
                state_ = State::BEFORE_THROW;
            }
        }
        break;
        case ( State::BEFORE_THROW ):
        {
            if ( timer_ >= 16 )
            {
                timer_ = 0;
                state_ = State::THROW;
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( State::THROW ):
        {
        }
        break;
        case ( State::AFTER_THROW ):
        {
            if ( timer_ >= 32 )
            {
                timer_ = 0;
                state_ = State::LOWERING;
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( State::LOWERING ):
        {
            moveDown();
            if ( hit_box_.y >= original_hit_box_.y - Unit::PixelsToSubPixels( 1 ) )
            {
                fullStopY();
                hit_box_.y = original_hit_box_.y;
                state_ = State::DELAYED;
            }
        }
        break;
        case ( State::DELAYED ):
        {
            if ( timer_ >= 16 )
            {
                timer_ = 0;
                state_ = State::DORMANT;
            }
            else
            {
                ++timer_;
            }
        }
        break;
    }
};

void AtticMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{

    if ( them.hasType( SpriteType::HERO ) )
    {
        switch ( state_ )
        {
            case ( State::DORMANT ):
            {
                if ( testIsNear( them ) )
                {
                    state_ = State::RISING;
                }
            }
            break;
            case ( State::THROW ):
            {
                const Direction::Horizontal direction = ( them.hit_box_.x > rightSubPixels() ) ? Direction::Horizontal::RIGHT : Direction::Horizontal::LEFT;
                level_state.sprites().spawn( std::unique_ptr<AtticMonsterProjectileSprite> ( new AtticMonsterProjectileSprite( centerXPixels(), centerYPixels(), direction ) ) );
                state_ = State::AFTER_THROW;
            }
            break;
            default:
            {
                // Do nothing.
            }
            break;
        }

        if ( state_ != State::DORMANT && state_ != State::DELAYED )
        {
            if ( their_collision.collideAny() )
            {
                if ( !them.onGround() )
                {
                    kill();
                    them.bounce();
                    if ( them.movementType() != SpriteMovement::Type::SWIMMING )
                    {
                        level_state.inventory().bop();
                    }
                    Audio::playSound( Audio::SoundType::BOP );
                }
                else
                {
                    level_state.health().hurt();
                }
            }
        }
    }
};

bool AtticMonsterSprite::testIsNear( const Sprite& them ) const
{
    return them.leftSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 8 ) &&
        them.rightSubPixels() > leftSubPixels() - Unit::BlocksToSubPixels( 8 ) &&
        them.topSubPixels() < bottomSubPixels() &&
        them.bottomSubPixels() > topSubPixels() - Unit::BlocksToSubPixels( 8 );
};

void AtticMonsterSprite::reset()
{
    vy_ = 0;
    acceleration_y_ = 0;
    is_dead_ = dead_no_animation_ = death_finished_ = false;
    changeMovement( SpriteMovement::Type::FLOATING );
    state_ = State::DORMANT;
    hit_box_ = original_hit_box_;
    sprite_interact_ = true;
};