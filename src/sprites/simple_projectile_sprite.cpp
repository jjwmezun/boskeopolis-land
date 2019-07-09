#include "simple_projectile_sprite.hpp"

SimpleProjectileSprite::SimpleProjectileSprite( int x, int y, int w, int h, Direction::Horizontal dir, std::unique_ptr<SpriteGraphics>&& gfx )
:
	Sprite( std::move( gfx ), x, y, w, h, { SpriteType::ENEMY }, 1600, 2400, 0, 0, dir, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false )
{};

SimpleProjectileSprite::~SimpleProjectileSprite() {};

void SimpleProjectileSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveInDirectionX();
};

void SimpleProjectileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};
