#include <cmath>
#include "devil_fire_sprite.hpp"
#include "sprite_graphics.hpp"

#define SPEED 6000.0

static constexpr int MAX_FRAMES = 8;
static int FRAMES[ MAX_FRAMES ] = { 0, 1, 2, 3, 3, 2, 1, 0 };

DevilFireSprite::DevilFireSprite( int x, int y, double dy, double dx, double start_speed )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/moon-monster.png", 32, 0, false, false, 0.0, -4, -4, 8, 8 ), x, y, 8, 8, { SpriteType::ENEMY }, 100, 500, 0, 0, ( dx < 0 ) ? Direction::Horizontal::LEFT : Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false, true ),
	angle_ ( std::atan2( dy, dx ) ),
    speed_ ( start_speed ),
    rotation_ ( ( dx < 0 ) ? -5 : 5 ),
    timer_ ( 0 ),
    animation_frame_ ( 0 )
{};

DevilFireSprite::~DevilFireSprite() {};

void DevilFireSprite::customUpdate( LevelState& level_state )
{
    speed_ += 25;
    if ( speed_ > 2000 )
    {
        speed_ = 2000;
    }
	hit_box_.x += ( int )( speed_ * std::cos( angle_ ) );
	hit_box_.y += ( int )( speed_ * std::sin( angle_ ) );
    graphics_->rotation_ += rotation_;
    ++timer_;
    if ( timer_ == 8 )
    {
        timer_ = 0;
        ++animation_frame_;
        if ( animation_frame_ == MAX_FRAMES )
        {
            animation_frame_ = 0;
        }
        graphics_->current_frame_x_ = ( ( int )( std::floor( FRAMES[ animation_frame_ ] / 2 ) ) * 16 ) + 32;
        graphics_->current_frame_y_ = ( FRAMES[ animation_frame_ ] % 2 ) * 16;
    }
};

void DevilFireSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
