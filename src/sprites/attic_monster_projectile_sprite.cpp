#include <cmath>
#include "attic_monster_projectile_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

#define SPEED 2000.0

AtticMonsterProjectileSprite::AtticMonsterProjectileSprite( int x, int y, double dx, double dy )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png", 0, 0, false, false, 0.0, true, -2, -2, 4, 4 ), x, y, 4, 4, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true, false ),
    angle_ ( std::atan2( dy, dx ) )
{};

AtticMonsterProjectileSprite::~AtticMonsterProjectileSprite() {};

void AtticMonsterProjectileSprite::customUpdate( LevelState& level_state )
{
	const double x_angle = std::cos( angle_ );
	hit_box_.x += ( int )( SPEED * x_angle );
	hit_box_.y += ( int )( SPEED * std::sin( angle_ ) );

	const double rotation = ( x_angle <= 0.0 ) ? -2.0 : 2.0;
	graphics_->rotation_ += rotation;
};

void AtticMonsterProjectileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
