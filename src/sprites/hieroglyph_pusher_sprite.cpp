#include "collision.hpp"
#include "hieroglyph_pusher_sprite.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

HieroglyphPusherSprite::HieroglyphPusherSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/hieroglyph_pusher.png" ), x, y, 16, 16, {}, 100, 1500, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

HieroglyphPusherSprite::~HieroglyphPusherSprite() {};

void HieroglyphPusherSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	turnOnEdge( blocks );
	turnOnCollide();
	moveInDirectionX();

	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
			graphics_->flip_x_ = true;
		break;
		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = false;
		break;
	}
};

void HieroglyphPusherSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		them.hit_box_.x += vx_;
		
		if
		(
			( them.collide_left_ && their_collision.collideRight() ) ||
			( them.collide_right_ && their_collision.collideLeft() )
		)
		{
			them.kill();
		}
	}
};