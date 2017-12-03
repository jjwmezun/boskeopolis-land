#include "mezun_math.hpp"
#include "snowball_sprite.hpp"
#include "sprite_graphics.hpp"

SnowballSprite::SnowballSprite( int x, int y, Direction::Horizontal dir )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/snowman.png", 0, 0, false, false, 0, false, -1, -1, 2, 2 ), x, y, 6, 6, { SpriteType::ENEMY }, 1600, 2400, 0, 0, dir, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false ),
	fall_speed_ ( mezun::randInt( 240, 60 ) ),
	counter_ ( 0 )
{};

SnowballSprite::~SnowballSprite() {};

void SnowballSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::RIGHT ):
			moveRight();
		break;

		case ( Direction::Horizontal::LEFT ):
			moveLeft();
		break;
	}
/*	
	if ( counter_ % fall_speed_ == 0 )
	{
		moveDown();
	}
	else
	{
		stopY();
	}*/

	++counter_;
};

void SnowballSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};