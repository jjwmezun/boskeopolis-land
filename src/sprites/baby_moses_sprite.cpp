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
    throw_dir_ ( Direction::Horizontal::__NULL ),
    acc2_ ( 0 ),
    vx2_ ( 0 )
{};

BabyMosesSprite::~BabyMosesSprite() {};

void BabyMosesSprite::customUpdate( LevelState& level_state )
{
    acc2_ = 0;
    if ( throw_ )
    {
        switch ( throw_dir_ )
        {
            case ( Direction::Horizontal::LEFT ):
            {
                acc2_ = -2000;
            }
            break;
            case ( Direction::Horizontal::RIGHT ):
            {
                acc2_ = 2000;
            }
            break;
        }
        acc2_ += ( throw_speed_ * 4 );
        throw_ = false;
    }
    vx2_ += acc2_;
    if ( acc2_ == 0 )
    {
        vx2_ /= ( ( on_ground_ ) ? 1.15 : 1.05 );
    }
    hit_box_.x += vx2_;
};

void BabyMosesSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( in_water_ )
        {
            them.kill();
        }
        else if ( held_ )
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
