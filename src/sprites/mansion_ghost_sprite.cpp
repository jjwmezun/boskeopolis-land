#include "mansion_ghost_sprite.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

static constexpr int LIGHT_LIMIT = 32;

MansionGhostSprite::MansionGhostSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/ghost.png", 0, 0, false, false, 0.0, false, -4, -4, 8, 8 ), x, y, 8, 8, { SpriteType::ENEMY, SpriteType::MANSION_GHOST }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	light_timer_ ( 0 )
{};

MansionGhostSprite::~MansionGhostSprite() {};

void MansionGhostSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	graphics_->current_frame_x_ = ( collide_left_ ) ? 16 : 0;
	if ( collide_left_ )
	{
		++light_timer_;
		if ( light_timer_ > LIGHT_LIMIT )
		{
			kill();
		}
	}
	else
	{
		--light_timer_;
		if ( light_timer_ < 0 )
		{
			light_timer_ = 0;
		}
	}
	std::cout<<light_timer_<<std::endl;
};

void MansionGhostSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) {};
