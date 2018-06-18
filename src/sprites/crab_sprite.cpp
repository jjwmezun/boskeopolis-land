#include "crab_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

CrabSprite::CrabSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/crab.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 100, 800, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

CrabSprite::~CrabSprite() {};

void CrabSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveInDirectionX();
};

void CrabSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.hit_box_.x > rightSubPixels() )
		{
			direction_x_ = Direction::Horizontal::RIGHT;
		}
		else
		{
			direction_x_ = Direction::Horizontal::LEFT;
		}
	}
};
