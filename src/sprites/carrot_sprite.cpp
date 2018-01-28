#include "carrot_sprite.hpp"
#include "sprite_graphics.hpp"
#include "unit.hpp"

#include <iostream>
CarrotSprite::CarrotSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/carrot.png", 0, 0, false, false, 0, false, -2, 0, 4, 2 ), x, y, 12, 22, { SpriteType::ENEMY }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	timer_ ( 0 )
{};

CarrotSprite::~CarrotSprite() {};

void CarrotSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	turnOnEdge( blocks );
	turnOnCollide();
	moveInDirectionX();

	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
	++timer_;
	if ( timer_ >= Unit::DEFAULT_ANIMATION_SPEED )
	{
		timer_ = 0 ;
		graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 0 ) ? 16 : 0;
	}
};

void CarrotSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};