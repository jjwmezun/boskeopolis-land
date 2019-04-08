#include "shmup_hero_bullet_sprite.hpp"
#include "sprite_graphics.hpp"

ShmupHeroBulletSprite::ShmupHeroBulletSprite( int x, int y, Direction::Simple direction, SpriteType type )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/shmup.png", 32, ( ( type == SpriteType::ENEMY ) ? 20 : 16 ) ), x, y, 5, 4, { type }, 1600, ( ( type == SpriteType::ENEMY ) ? 6000 : 3000 ), 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false, true ),
	direction_ ( direction )
{};

ShmupHeroBulletSprite::~ShmupHeroBulletSprite() {};

void ShmupHeroBulletSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
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

void ShmupHeroBulletSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};
