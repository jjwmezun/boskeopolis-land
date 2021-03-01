#include "audio.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "horizontal_space_beam_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

HorizontalSpaceBeamSprite::HorizontalSpaceBeamSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/space-beam.png" ), x - Unit::BlocksToPixels( 100 ), y, Unit::BlocksToPixels( 100 ), 0, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::UP, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    beam_state_ ( BeamState::INACTIVE ),
    timer_ ( 0 )
{
    graphics_->alpha_ = 200;
};

HorizontalSpaceBeamSprite::~HorizontalSpaceBeamSprite() {};

void HorizontalSpaceBeamSprite::updateHeight( int height )
{
    hit_box_.h = height;
    graphics_->current_frame_y_ = ( int )( ( double )( Unit::BlocksToPixels( 6 ) - heightPixels() ) / 2.0 );
    hit_box_.y = ( ( direction_y_ == Direction::Vertical::UP ) ? original_hit_box_.y : original_hit_box_.y + Unit::BlocksToSubPixels( 2 ) ) +
        Unit::PixelsToSubPixels( graphics_->current_frame_y_ );
};

void HorizontalSpaceBeamSprite::customUpdate( LevelState& level_state )
{
    switch ( beam_state_ )
    {
        case ( BeamState::INACTIVE ):
        {
            // Do nothing.
        }
        break;
        case ( BeamState::WAITING ):
        {
            if ( timer_ == 8 )
            {
                timer_ = 0;
                beam_state_ = BeamState::WARNING;
                updateHeight( 1000 );
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( BeamState::WARNING ):
        {
            if ( timer_ == 32 )
            {
                timer_ = 0;
                beam_state_ = BeamState::GROWING;
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( BeamState::GROWING ):
        {
            if ( hit_box_.h >= Unit::BlocksToSubPixels( 6 ) )
            {
                updateHeight( Unit::BlocksToSubPixels( 6 ) );
                beam_state_ = BeamState::SHOOTING;
                Audio::playSound( Audio::SoundType::LIGHTNING );
                timer_ = 0;
            }
            else
            {
                timer_ += 1000;
                if ( timer_ > 8000 )
                {
                    timer_ = 8000;
                }
                updateHeight( hit_box_.h + timer_ );
            }
        }
        break;
        case ( BeamState::SHOOTING ):
        {
            if ( timer_ == 8 )
            {
                timer_ = 0;
                beam_state_ = BeamState::SHRINKING;
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( BeamState::SHRINKING ):
        {
            if ( hit_box_.h <= 0 )
            {
                updateHeight( 0 );
                beam_state_ = BeamState::WAITING;
                direction_y_ = Direction::switchVertical( direction_y_ );
                timer_ = 0;
            }
            else
            {
                timer_ += 1000;
                if ( timer_ > 8000 )
                {
                    timer_ = 8000;
                }
                updateHeight( hit_box_.h - timer_ );
            }
        }
        break;
    }
};

void HorizontalSpaceBeamSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        switch ( beam_state_ )
        {
            case ( BeamState::INACTIVE ):
            {
                if
                (
                    them.hit_box_.y < original_hit_box_.bottom() + Unit::BlocksToSubPixels( 2 ) &&
                    them.bottomSubPixels() > original_hit_box_.y &&
                    them.rightSubPixels() > original_hit_box_.x + Unit::BlocksToSubPixels( 15 ) &&
                    them.hit_box_.x < original_hit_box_.x + Unit::BlocksToSubPixels( 101 )
                )
                {
                    beam_state_ = BeamState::WAITING;
                }
            }
            break;
            case ( BeamState::WAITING ):
            {
                if
                (
                    them.hit_box_.y > original_hit_box_.y + Unit::BlocksToSubPixels( 9 ) ||
                    them.bottomSubPixels() < original_hit_box_.y - Unit::BlocksToSubPixels( 1 )
                )
                {
                    beam_state_ = BeamState::INACTIVE;
                }
            }
            break;
            case ( BeamState::SHOOTING ):
            {
                if ( their_collision.collideAny() )
                {
                    level_state.health().hurt();
                }
            }
            break;
        }
    }
};
