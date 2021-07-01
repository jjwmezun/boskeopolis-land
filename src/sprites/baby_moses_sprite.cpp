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
    throw_ ( false ),
    throw_speed_ ( 0 ),
    throw_dir_ ( Direction::Horizontal::__NULL )
{};

BabyMosesSprite::~BabyMosesSprite() {};

void BabyMosesSprite::customUpdate( LevelState& level_state )
{
    if ( vx_ > 0 || vx_ < 0 )
    {
        vx_ /= 1.001;
    }
    if ( acceleration_x_ > 0 || acceleration_x_ < 0 )
    {
        acceleration_x_ = 0;
    }
    if ( throw_ )
    {
        switch ( throw_dir_ )
        {
            case ( Direction::Horizontal::LEFT ):
            {
                if ( throw_speed_ < -2000 )
                {
                    acceleration_x_ = -2000;
                }
                else
                {
                    acceleration_x_ = -1000;
                }
            }
            break;
            case ( Direction::Horizontal::RIGHT ):
            {
                if ( throw_speed_ > 2000 )
                {
                    acceleration_x_ = 2000;
                }
                else
                {
                    acceleration_x_ = 1000;
                }
            }
            break;
        }
        top_speed_walk_ = acceleration_x_;
        throw_ = false;
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
                throw_ = true;
                throw_speed_ = them.vx_;
                throw_dir_ = them.direction_x_;
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
