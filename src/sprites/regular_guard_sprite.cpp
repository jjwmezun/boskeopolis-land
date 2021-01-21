#include "regular_guard_sprite.hpp"
#include "sprite_graphics.hpp"

RegularGuardSprite::RegularGuardSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 2000, 2000, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

RegularGuardSprite::~RegularGuardSprite() {};

void RegularGuardSprite::customUpdate( LevelState& level_state )
{
    moveInDirectionX();
};

void RegularGuardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
