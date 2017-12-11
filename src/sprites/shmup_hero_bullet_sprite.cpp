#include "shmup_hero_bullet_sprite.hpp"
#include "sprite_graphics.hpp"

ShmupHeroBulletSprite::ShmupHeroBulletSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 32, 16 ), x, y, 5, 4, { SpriteType::HEROS_BULLET }, 1600, 3000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false, true )
{};

ShmupHeroBulletSprite::~ShmupHeroBulletSprite() {};

void ShmupHeroBulletSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveRight();
};

void ShmupHeroBulletSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};