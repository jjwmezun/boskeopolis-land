#include "collision.hpp"
#include "health.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "pollo_del_aire_still_sprite.hpp"
#include "sprite_graphics.hpp"

PolloDelAireStillSprite::PolloDelAireStillSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo_no_noko.png", 0, 32, false, false, 0, false, -1, -6, 2, 8 ), x, y, 22, 22, {}, 250, 500, 500, 4000, Direction::Horizontal::__NULL, Direction::Vertical::UP, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_counter_ ( 0 )
{};

PolloDelAireStillSprite::~PolloDelAireStillSprite() {};

void PolloDelAireStillSprite::deathAction( Camera& camera )
{
	PolloDelAireSprite::polloDeath( camera, *this );
};

void PolloDelAireStillSprite::reset()
{
	PolloDelAireSprite::polloReset( *this );
};

void PolloDelAireStillSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	
	if ( direction_y_ == Direction::Vertical::UP )
	{
		moveUp();
	}
	else
	{
		moveDown();
	}
	
	if ( hit_box_.y < original_hit_box_.y - 3000 )
	{
		direction_y_ = Direction::Vertical::DOWN;
	}
	else if ( hit_box_.y > original_hit_box_.y + 3000 )
	{
		direction_y_ = Direction::Vertical::UP;
	}

	++animation_counter_;
	if ( animation_counter_ >= 16 ) animation_counter_ = 0;
	graphics_->current_frame_x_ = 24 * floor(animation_counter_ / 8 );
};

void PolloDelAireStillSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	PolloDelAireSprite::polloInteract( my_collision, their_collision, them, blocks, sprites, lvmap, health, *this );
};