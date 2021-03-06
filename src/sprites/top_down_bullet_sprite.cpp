#include "top_down_bullet_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

TopDownBulletSprite::TopDownBulletSprite( int x, int y, Direction::Simple direction, Sprite::SpriteType type, Unit::Layer layer )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/top-down-bullet.png" ), x, y, 4, 4, { type }, 800, 3000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true, false ),
	direction_ ( direction )
{
	layer_ = layer;
};

TopDownBulletSprite::~TopDownBulletSprite() {};

void TopDownBulletSprite::customUpdate( LevelState& level_state )
{
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		{
			moveUp();
		}
		break;
		case ( Direction::Simple::RIGHT ):
		{
			moveRight();
		}
		break;
		case ( Direction::Simple::DOWN ):
		{
			moveDown();
		}
		break;
		case ( Direction::Simple::LEFT ):
		{
			moveLeft();
		}
		break;
	}
};

void TopDownBulletSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
