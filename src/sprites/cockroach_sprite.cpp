#include "level_state.hpp"
#include "cockroach_sprite.hpp"
#include "sprite_graphics.hpp"

CockroachSprite::CockroachSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png", 0, 0, false, false, 0.0, false, -1, -1, 2, 2 ), x, y, 32, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 2000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

CockroachSprite::~CockroachSprite() {};

void CockroachSprite::customUpdate( LevelState& level_state )
{
	turnOnEdge( level_state.blocks() );
	turnOnCollide();
	moveInDirectionX();
};

void CockroachSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
