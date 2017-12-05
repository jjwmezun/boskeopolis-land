#include "shmup_shooter_sprite.hpp"
#include "sprite_graphics.hpp"

ShmupShooterSprite::ShmupShooterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 80, 0, false, false, 0, false, -1, -1, 2, 2 ), x, y, 14, 14, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

ShmupShooterSprite::~ShmupShooterSprite() {};

void ShmupShooterSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void ShmupShooterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};