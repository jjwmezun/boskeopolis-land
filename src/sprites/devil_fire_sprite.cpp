#include <cmath>
#include "devil_fire_sprite.hpp"
#include "sprite_graphics.hpp"

#define SPEED 6000.0

DevilFireSprite::DevilFireSprite( int x, int y, double dy, double dx, double start_speed )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png", 0, 0, false, false, 0.0, -4, -4, 8, 8 ), x, y, 8, 8, { SpriteType::ENEMY }, 100, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false, true ),
	angle_ ( std::atan2( dy, dx ) ),
    speed_ ( start_speed )
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
};

void DevilFireSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
