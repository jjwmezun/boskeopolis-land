#include "olive_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>
#include "main.hpp"

static constexpr int SPEED = 800;

OliveSprite::OliveSprite( int x, int y, Direction::Horizontal start_dir )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/olive.png", 0, 0, false, false, 0.0, false, -2, -2, 4, 4 ), x, y, 12, 12, { SpriteType::ENEMY }, SPEED, SPEED, 0, 0, start_dir, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, true, true, true )
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