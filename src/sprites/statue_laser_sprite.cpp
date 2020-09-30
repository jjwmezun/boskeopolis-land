#include "audio.hpp"
#include "camera.hpp"
#include "statue_laser_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

StatueLaserSprite::StatueLaserSprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    laser_state_ ( LaserState::DORMANT ),
    timer_ ( 0 )
{};

StatueLaserSprite::~StatueLaserSprite() {};

void StatueLaserSprite::customUpdate( LevelState& level_state )
{
    switch ( laser_state_ )
    {
        case ( LaserState::START_CHARGING ):
        {
            changeStateAfterDelay( 8, LaserState::CHARGING );
        }
        break;
        case ( LaserState::CHARGING ):
        {
            changeStateAfterDelay( 8, LaserState::CHARGING2 );
        }
        break;
        case ( LaserState::CHARGING2 ):
        {
            ++timer_;
            if ( timer_ == 8 )
            {
                timer_ = 0;
                laser_state_ = LaserState::SHOOTING;
                Audio::playSound( Audio::SoundType::LIGHTNING );
            }
        }
        break;
        case ( LaserState::START_FADING ):
        {
            changeStateAfterDelay( 8, LaserState::FADING );
        }
        break;
        case ( LaserState::FADING ):
        {
            changeStateAfterDelay( 8, LaserState::FADING2 );
        }
        break;
        case ( LaserState::FADING2 ):
        {
            changeStateAfterDelay( 8, LaserState::DORMANT );
        }
        break;
    }
};

void StatueLaserSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        switch ( laser_state_ )
        {
            case ( LaserState::DORMANT ):
            {
                if ( level_state.camera().onscreen( hit_box_ ) && testInFrontOfLaser( them ) )
                {
                    laser_state_ = LaserState::START_CHARGING;
                }
            }
            break;
            case ( LaserState::SHOOTING ):
            {
                if ( level_state.camera().offscreen( hit_box_ ) || !testInFrontOfLaser( them ) )
                {
                    laser_state_ = LaserState::START_FADING;
                }
                else if ( testTouchingLaser( them ) )
                {
                    level_state.health().hurt();
                }
            }
            break;
        }
    }
};

void StatueLaserSprite::render( const Camera& camera ) const
{
    if ( laser_state_ == LaserState::DORMANT || laser_state_ == LaserState::START_CHARGING )
    {
        return;
    }

    const int laser_height = ( laser_state_ == LaserState::SHOOTING )
        ? 8
        : ( laser_state_ == LaserState::CHARGING2 || laser_state_ == LaserState::FADING2 )
            ? 4
            : 2;
    const int laser_y = centerYPixels() - ( int )( std::floor( ( double )( laser_height ) / 2.0 ) );
    Render::renderRect( { 0, laser_y, camera.relativeX( xPixels() ), laser_height }, 1 );
    Render::renderRectDebug( Unit::SubPixelsToPixels( hit_box_ ), { 255, 0, 0, 128 } );
};

bool StatueLaserSprite::testInFrontOfLaser( const Sprite& them ) const
{
    return them.hit_box_.y <= bottomSubPixels() && them.bottomSubPixels() >= hit_box_.y && them.rightSubPixels() < hit_box_.x;
};

bool StatueLaserSprite::testTouchingLaser( const Sprite& them ) const
{
    return them.hit_box_.y + Unit::PixelsToSubPixels( 4 ) <= bottomSubPixels() && them.hit_box_.y + Unit::PixelsToSubPixels( 8 ) >= hit_box_.y && them.rightSubPixels() < hit_box_.x;
};

void StatueLaserSprite::changeStateAfterDelay( int delay, LaserState next_state )
{
    ++timer_;
    if ( timer_ == delay )
    {
        timer_ = 0;
        laser_state_ = next_state;
    }
};