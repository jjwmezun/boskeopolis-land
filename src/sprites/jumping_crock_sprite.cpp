#include "jumping_crock_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "sprite_graphics.hpp"

JumpingCrockSprite::JumpingCrockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 24, { SpriteType::ENEMY }, 100, 3000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    crock_state_ ( CrockState::STILL )
{};

JumpingCrockSprite::~JumpingCrockSprite() {};

void JumpingCrockSprite::customUpdate( LevelState& level_state )
{
    switch ( crock_state_ )
    {
        case ( CrockState::STILL ):
        {
            crock_state_ = CrockState::JUMPING;
        }
        break;
        case ( CrockState::JUMPING ):
        {
            moveUp();
            if ( hit_box_.y <= original_hit_box_.y - Unit::BlocksToSubPixels( 8 ) )
            {
                crock_state_ = CrockState::CRESTING;
            }
        }
        break;
        case ( CrockState::CRESTING ):
        {
            stopY();
            moveLeft();
            if ( hit_box_.x <= original_hit_box_.x - Unit::PixelsToSubPixels( 8 ) )
            {
                crock_state_ = CrockState::FALLING;
            }
        }
        break;
        case ( CrockState::FALLING ):
        {
            stopX();
            moveDown();
            if ( bottomSubPixels() + Unit::BlocksToSubPixels( 1 ) >= Unit::BlocksToSubPixels( level_state.currentMap().heightBlocks() ) )
            {
                crock_state_ = CrockState::STILL;
                hit_box_.x = original_hit_box_.x;
            }
        }
        break;
    }
};

void JumpingCrockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
