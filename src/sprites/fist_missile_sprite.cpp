#include "fist_missile_sprite.hpp"
#include "sprite_graphics.hpp"

FistMissileSprite::FistMissileSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/handgun.png", 0, 0, false, false, 0.0, -1, -1, 2, 2 ), x, y + 2, 28, 28, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 200, 3000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true )
{};

FistMissileSprite::~FistMissileSprite() {};

void FistMissileSprite::customUpdate( LevelState& level_state )
{
	moveLeft();
	if ( collide_left_ )
	{
		kill();
	}
};

void FistMissileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
