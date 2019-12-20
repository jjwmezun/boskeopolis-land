#include <cmath>
#include "bridge_monster_projectile_sprite.hpp"
#include "sprite_graphics.hpp"

#define SPEED 3000.0

BridgeMonsterProjectileSprite::BridgeMonsterProjectileSprite( int x, int y, double dy, double dx )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 8, 8, { SpriteType::ENEMY }, 100, 500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false, true ),
	angle_ ( std::atan2( dy, dx ) )
{};

BridgeMonsterProjectileSprite::~BridgeMonsterProjectileSprite() {};

void BridgeMonsterProjectileSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	hit_box_.x += ( int )( SPEED * std::cos( angle_ ) );
	hit_box_.y += ( int )( SPEED * std::sin( angle_ ) );
};

void BridgeMonsterProjectileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};
