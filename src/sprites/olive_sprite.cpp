#include "olive_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>
#include "main.hpp"

OliveSprite::OliveSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/olive.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 1000, 1000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, true, true, true )
{};

OliveSprite::~OliveSprite() {};

void OliveSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	turnOnCollide();
	moveInDirectionX();
	
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::RIGHT ):
			graphics_->rotation_ += 5;
		break;
		case ( Direction::Horizontal::LEFT ):
			graphics_->rotation_ -= 5;
		break;
	}
	
	if ( fellInBottomlessPit( lvmap ) )
	{
		kill();
	}
};

void OliveSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
};