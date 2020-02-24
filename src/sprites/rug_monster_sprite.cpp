#include "audio.hpp"
#include "rug_monster_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int NUMBER_OF_ANIMATION_FRAMES = 4;
static constexpr int TICKS_PER_FRAME = 8;

RugMonsterSprite::RugMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/rug-monster.png", 0, 0, false, false, 0.0, false, -1, -1, 2, 1 ), x, y + 11, 14, 5, {}, 100, 2000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    reached_end_ ( false ),
    state_ ( RugMonsterState::DORMANT ),
    animation_timer_ ( 0 ),
    current_animation_frame_ ( 0 )
{};

RugMonsterSprite::~RugMonsterSprite() {};

void RugMonsterSprite::customUpdate( LevelState& level_state )
{
    switch ( state_ )
    {
        case ( RugMonsterState::RISING ):
        {
            if ( current_animation_frame_ >= NUMBER_OF_ANIMATION_FRAMES - 1 )
            {
                state_ = RugMonsterState::UP;
                break;
            }
            else
            {
                if ( animation_timer_ >= TICKS_PER_FRAME - 1 )
                {
                    ++current_animation_frame_;
                    graphics_->current_frame_x_ = 16 * current_animation_frame_;
                    animation_timer_ = 0;
                }
                else
                {
                    ++animation_timer_;
                }
            }  
        }
        break;

        case ( RugMonsterState::UP ):
        {
            moveInDirectionX();
            switch ( direction_x_ )
            {
                case ( Direction::Horizontal::LEFT ):
                {
                    if ( hit_box_.x < original_hit_box_.x - Unit::BlocksToSubPixels( 12 ) )
                    {
                        direction_x_ = Direction::Horizontal::RIGHT;
                        reached_end_ = true;
                    }
                }
                break;

                case ( Direction::Horizontal::RIGHT ):
                {
                    if ( hit_box_.x > original_hit_box_.x - Unit::BlocksToSubPixels( 1 ) )
                    {
                        direction_x_ = Direction::Horizontal::LEFT;
                        reached_end_ = true;
                    }
                }
                break;
            }
        }
        break;

        case ( RugMonsterState::FALLING ):
        {
            stopX();
            if ( current_animation_frame_ <= 0 )
            {
                state_ = RugMonsterState::CHANGING;
                break;
            }
            else
            {
                if ( animation_timer_ >= TICKS_PER_FRAME - 1 )
                {
                    --current_animation_frame_;
                    graphics_->current_frame_x_ = 16 * current_animation_frame_;
                    animation_timer_ = 0;
                }
                else
                {
                    ++animation_timer_;
                }
            }
        }
        break;
    }
};

void RugMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( !is_dead_ && them.hasType( SpriteType::HERO ) )
    {
        switch ( state_ )
        {
            case ( RugMonsterState::DORMANT ):
            {
                if ( them.hasType( SpriteType::HERO ) )
                {
                    if
                    (
                        them.rightSubPixels() > hit_box_.x - Unit::BlocksToSubPixels( 6 ) &&
                        them.hit_box_.x < rightSubPixels() + Unit::BlocksToSubPixels( 6 ) &&
                        them.bottomSubPixels() > hit_box_.y - Unit::BlocksToSubPixels( 4 ) &&
                        them.hit_box_.y < bottomSubPixels() + Unit::BlocksToSubPixels( 4 )
                    )
                    {
                        state_ = RugMonsterState::RISING;
                    }
                }
            }
            break;

            case ( RugMonsterState::UP ):
            {
                if ( them.collideBottomOnly( their_collision, *this ) )
                {
                    kill();
                    them.bounce();
                    level_state.inventory().bop();
                    Audio::playSound( Audio::SoundType::BOP );
                }
                else if ( their_collision.collideAny() )
                {
                    level_state.health().hurt();
                }

                if ( reached_end_ )
                {
                    if
                    (
                        them.rightSubPixels() < hit_box_.x - Unit::BlocksToSubPixels( 8 ) ||
                        them.hit_box_.x > rightSubPixels() + Unit::BlocksToSubPixels( 8 ) ||
                        them.bottomSubPixels() < hit_box_.y - Unit::BlocksToSubPixels( 8 ) ||
                        them.hit_box_.y > bottomSubPixels() + Unit::BlocksToSubPixels( 8 )
                    )
                    {
                        state_ = RugMonsterState::FALLING;
                    }
                    reached_end_ = false;
                }
            }
            break;

            case ( RugMonsterState::CHANGING ):
            {
                do
                {
                    hit_box_.x = mezun::randInt( original_hit_box_.x, original_hit_box_.x - Unit::BlocksToSubPixels( 13 ) );
                }
                while
                (
                    them.rightSubPixels() > hit_box_.x - Unit::BlocksToSubPixels( 4 ) &&
                    them.hit_box_.x < rightSubPixels() + Unit::BlocksToSubPixels( 4 )
                );
                state_ = RugMonsterState::RISING;
            }
            break;
        }
    }
};

void RugMonsterSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
    stopX();
    if ( current_animation_frame_ <= 0 )
    {
        death_finished_ = true;
    }
    else
    {
        if ( animation_timer_ >= TICKS_PER_FRAME - 1 )
        {
            --current_animation_frame_;
            graphics_->current_frame_x_ = 16 * current_animation_frame_;
            animation_timer_ = 0;
        }
        else
        {
            ++animation_timer_;
        }
    }
};