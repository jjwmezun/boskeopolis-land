#include "collision.hpp"
#include "rotating_block_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

RotatingBlockSprite::RotatingBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 32, 32, {}, 100, 600, 1000, 1000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

RotatingBlockSprite::~RotatingBlockSprite() {};

void RotatingBlockSprite::customUpdate( LevelState& level_state )
{
	graphics_->rotation_ -= 1;
	while ( graphics_->rotation_ <= -90 ) graphics_->rotation_ += 90;
	std::cout<<graphics_->rotation_<<std::endl;
};

void RotatingBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
	}
};
