#include "audio.hpp"
#include "collision.hpp"
#include "bull_sprite.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

BullSprite* BullSprite::generateRegular( int x, int y, Direction::Horizontal dir )
{
    return new BullSprite( x, y, dir, BullState::AWAKE, 50, 6000, CameraMovement::RESET_OFFSCREEN_AND_AWAY );
};

BullSprite* BullSprite::generateWaking( int x, int y, Direction::Horizontal dir )
{
    return new BullSprite( x, y, dir, BullState::SLEEPING, 100, 6000, CameraMovement::RESET_OFFSCREEN_AND_AWAY );
};

BullSprite* BullSprite::generateSuper( int x, int y, Direction::Horizontal dir )
{
    return new BullSprite( x, y, dir, BullState::SLEEPING, 100, 8000, CameraMovement::PERMANENT );
};

BullSprite::BullSprite( int x, int y, Direction::Horizontal dir, BullState bull_state, int start_speed, int top_speed, CameraMovement camera_movement )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 56, 40, { SpriteType::BULL }, start_speed, top_speed, 0, 0, dir, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, camera_movement ),
    bull_state_ ( bull_state )
{};

BullSprite::~BullSprite() {};

void BullSprite::customUpdate( LevelState& level_state )
{
    switch ( bull_state_ )
    {
        case ( BullState::SLEEPING ):
        {
        }
        break;
        case ( BullState::AWAKE ):
        {
            moveInDirectionX();
            turnOnCollide();
        }
        break;
    }
};

void BullSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( them.collideBottomOnly( their_collision, *this ) )
        {
            them.bounce();
            Audio::playSound( Audio::SoundType::BOP );
        }
        else if ( their_collision.collideAny() )
        {
            level_state.health().hurt();
        }

        switch ( bull_state_ )
        {
            case ( BullState::SLEEPING ):
            {
                // If within bull’s sights, wake up.
                if
                (
                    them.hit_box_.y < bottomSubPixels() && them.bottomSubPixels() > hit_box_.y &&
                    (
                        ( direction_x_ == Direction::Horizontal::LEFT )
                            ? them.rightSubPixels() < hit_box_.x
                            : them.hit_box_.x > rightSubPixels()
                    )
                )
                {
                    bull_state_ = BullState::AWAKE;
                }
            }
            break;
            case ( BullState::AWAKE ):
            {
                // Make bull move in direction o’ player.
                if ( them.hit_box_.x > rightSubPixels() )
                {
                    direction_x_ = Direction::Horizontal::RIGHT;
                }
                else if ( them.rightSubPixels() < hit_box_.x )
                {
                    direction_x_ = Direction::Horizontal::LEFT;
                }
            }
            break;
        }
    }
};
