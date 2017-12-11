#include <cmath>
#include "shmup_bullet_sprite.hpp"
#include "sprite_graphics.hpp"

#define SPEED 4000.0

ShmupBulletSprite::ShmupBulletSprite( int x, int y, double dy, double dx )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 37, 16 ), x, y, 8, 8, { SpriteType::ENEMY }, 100, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false, true ),
	angle_ ( std::atan2( dy, dx ) )
{};

ShmupBulletSprite::~ShmupBulletSprite() {};

void ShmupBulletSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	hit_box_.x += ( int )( SPEED * std::cos( angle_ ) );
	hit_box_.y += ( int )( SPEED * std::sin( angle_ ) );
};

void ShmupBulletSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};