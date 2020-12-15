#include "collision.hpp"
#include "hopping_clock_sprite.hpp"
#include "sprite_graphics.hpp"

HoppingClockSprite::HoppingClockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bad_apple.png" ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 400, 1600, 500, 3000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{
    jump_lock_ = false;
};

HoppingClockSprite::~HoppingClockSprite() {};

void HoppingClockSprite::customUpdate( LevelState& level_state )
{
	moveInDirectionX();
	turnOnCollide();
    jump();
};

void HoppingClockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
