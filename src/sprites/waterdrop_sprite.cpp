#include "collision.hpp"
#include "health.hpp"
#include "waterdrop_sprite.hpp"
#include "waterdrop_graphics.hpp"
#include <iostream>

WaterdropSprite::WaterdropSprite( int x, int y )
:
	Sprite( std::make_unique<WaterdropGraphics> (), x, y, 14, 16, {}, 100, 1500, 400, 2400, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true )
{};

WaterdropSprite::~WaterdropSprite() {};

void WaterdropSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{	
	moveDown();
};

void WaterdropSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( their_collision.collideAny() )
	{
		if ( them.hasType( SpriteType::HERO ) && !is_dead_ )
		{
			health.hurt();
		}

		kill();
	}
};

void WaterdropSprite::deathAction( Camera& camera )
{
	fullStopY();

	if ( graphics_->current_frame_x_ >= 5 * 14 )
	{
		death_finished_ = true;
	}
};