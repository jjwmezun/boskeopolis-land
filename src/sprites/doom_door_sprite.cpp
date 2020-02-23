#include "doom_door_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

DoomDoorSprite::DoomDoorSprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

DoomDoorSprite::~DoomDoorSprite() {};

void DoomDoorSprite::customUpdate( LevelState& level_state )
{};

void DoomDoorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};
