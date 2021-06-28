#include "baby_moses_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

BabyMosesSprite::BabyMosesSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 16, { SpriteType::BABY_MOSES }, 0, 5000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    held_ ( false ),
    throw_ ( 0 )
{};

BabyMosesSprite::~BabyMosesSprite() {};

void BabyMosesSprite::customUpdate( LevelState& level_state )
{
    if ( vx_ > 0 )
    {
        vx_ /= 1.001;
    }
    if ( acceleration_x_ > 0 )
    {
        acceleration_x_ = 0;
    }
    if ( throw_ != 0 )
    {
        top_speed_walk_ = acceleration_x_ = std::max( -16000, std::min( 16000, 100 * throw_ ) );
        throw_ = 0;
    }
};

void BabyMosesSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( held_ )
        {
            hit_box_.x = them.hit_box_.x;
            hit_box_.y = them.hit_box_.y - Unit::PixelsToSubPixels( 16 );
            if ( !them.is_ducking_ && Input::pressed( Input::Action::RUN ) )
            {
                held_ = false;
                changeMovement( SpriteMovement::Type::GROUNDED );
                throw_ = them.vx_;
                printf( "%d\n", throw_ );
            }
        }
        else if
        (
            them.is_ducking_ &&
            Input::pressed( Input::Action::RUN ) &&
            their_collision.collideAny()
        )
        {
            //level_state.events().setPlayerHolding( system_id_ );
            held_ = true;
            changeMovement( SpriteMovement::Type::FLOATING );
        }
    }
};
