#include "collision.hpp"
#include "quadrapus_sprite.hpp"
#include "sprite_graphics.hpp"

QuadrapusSprite::QuadrapusSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/quadrapus.png", 0, 0, false, false, 0, -1, -21, 2, 22 ), x, y, 30, 10, { SpriteType::ENEMY }, 100, 2000, 400, 5000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false )
{};

QuadrapusSprite::~QuadrapusSprite() {};

void QuadrapusSprite::customUpdate( LevelState& level_state )
{
};

void QuadrapusSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.rightSubPixels() < xSubPixels() )
		{
			moveLeft();
		}
		else if ( them.xSubPixels() > rightSubPixels() )
		{
			moveRight();
		}

		if ( them.bottomSubPixels() < ySubPixels() )
		{
			moveUp();
		}
		else if ( them.ySubPixels() > bottomSubPixels() )
		{
			moveDown();
		}
	}
};
