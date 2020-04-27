#include "thrower_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

ThrowerSprite::ThrowerSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 24, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

ThrowerSprite::~ThrowerSprite() {};

void ThrowerSprite::customUpdate( LevelState& level_state )
{};

void ThrowerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
