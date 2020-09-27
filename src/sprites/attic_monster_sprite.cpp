#include "attic_monster_sprite.hpp"
#include "attic_monster_projectile_sprite.hpp"
#include "audio.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "inventory.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

AtticMonsterSprite::AtticMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/attic-rat.png", 0, 0, false, false, 0.0, false, -4, -7, 7, 8 ), x + 4, y + 16, 15, 0, {}, 100, 2000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
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
            hit_box_.h += Unit::PixelsToSubPixels( 1 );
            if ( hit_box_.h >= Unit::PixelsToSubPixels( 16 ) )
            {
                hit_box_.h = Unit::PixelsToSubPixels( 16 );
                state_ = State::BEFORE_THROW;
            }
            hit_box_.y = original_hit_box_.y - hit_box_.h;
        }
        break;
        case ( State::BEFORE_THROW ):
        {
            if ( timer_ >= 16 )
            {
                timer_ = 0;
                state_ = State::THROW;
                graphics_->current_frame_x_ = 66;
            }
            else
            {
                ++timer_;
                if ( timer_ == 8 )
                {
                    graphics_->current_frame_x_ = 22;
                }
                else if ( timer_ == 12 )
                {
                    graphics_->current_frame_x_ = 44;
                }
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
                if ( timer_ == 4 )
                {
                    graphics_->current_frame_x_ = 0;
                }
            }
        }
        break;
        case ( State::LOWERING ):
        {
            hit_box_.h -= Unit::PixelsToSubPixels( 1 );
            if ( hit_box_.h <= 0 )
            {
                hit_box_.h = 0;
                state_ = State::DELAYED;
            }
            hit_box_.y = original_hit_box_.y - hit_box_.h;
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
    if ( !isDead() && them.hasType( SpriteType::HERO ) )
    {
        direction_x_ = ( them.hit_box_.x > rightSubPixels() ) ? Direction::Horizontal::RIGHT : Direction::Horizontal::LEFT;
        flipGraphicsOnRight();
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
                level_state.sprites().spawn( std::unique_ptr<AtticMonsterProjectileSprite> ( new AtticMonsterProjectileSprite( centerXPixels(), centerYPixels(), direction_x_ ) ) );
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
    if ( !isDead() )
    {
        vy_ = 0;
        acceleration_y_ = 0;
        is_dead_ = dead_no_animation_ = death_finished_ = false;
        changeMovement( SpriteMovement::Type::FLOATING );
        state_ = State::DORMANT;
        hit_box_ = original_hit_box_;
        sprite_interact_ = true;
    }
};

void AtticMonsterSprite::render( Camera& camera, bool priority )
{
    if ( !priority )
    {
        Render::renderObject( "sprites/attic-rat.png", { 88, 18, 22, 3 }, { camera.relativeX( xPixels() - 4 ), camera.relativeY( Unit::SubPixelsToPixels( original_hit_box_.y ) - 3 ), 22, 3 } );
        graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
        Render::renderObject( "sprites/attic-rat.png", { 88, 21, 22, 3 }, { camera.relativeX( xPixels() - 4 ), camera.relativeY( Unit::SubPixelsToPixels( original_hit_box_.y ) - 1 ), 22, 3 } );
    }
};

void AtticMonsterSprite::deathAction( LevelState& level_state )
{
    graphics_->current_frame_x_ = 110;
    hit_box_.h -= Unit::PixelsToSubPixels( 1 );
    if ( hit_box_.h <= -8000 )
    {
        hit_box_.h = -8000;
    }
    hit_box_.y = original_hit_box_.y - hit_box_.h;
};