#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "pollo_sprite.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "sprite_graphics.hpp"

PolloSprite::PolloSprite( int x, int y, Direction::Horizontal dir_x, int map_id, bool despawn_when_dead )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo_no_noko.png", 0, 0, ( dir_x == Direction::Horizontal::RIGHT ), false, 0, -1, -6, 2, 7 ), x, y, 22, 22, { SpriteType::DEATH_COUNT }, 500, 3000, 500, 4000, dir_x, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, despawn_when_dead, true, true, .2, map_id ),
	switch_from_ ( Direction::Horizontal::__NULL )
{
	jump_lock_ = false;
};

PolloSprite::~PolloSprite() {};

void PolloSprite::customUpdate( LevelState& level_state )
{
	jump();

	if ( direction_x_ == Direction::Horizontal::RIGHT )
	{
		moveRight();
	}
	else
	{
		moveLeft();
	}

	if ( ( graphics_->current_frame_x_ == 48 || graphics_->current_frame_x_ == 72 ) && switch_from_ != Direction::Horizontal::__NULL )
	{
		direction_x_ = Direction::switchHorizontal( switch_from_ );
		graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
		switch_from_ = Direction::Horizontal::__NULL;
	}
	else if ( collidedRight() || collidedLeft() )
	{
		if ( graphics_->current_frame_x_ == 0 ) graphics_->current_frame_x_ = 48;
		else if ( graphics_->current_frame_x_ == 24 ) graphics_->current_frame_x_ = 72;
		switch_from_ = ( collidedLeft() ) ? Direction::Horizontal::LEFT : Direction::Horizontal::RIGHT;
	}
	else if ( is_jumping_ )
	{
		graphics_->current_frame_x_ = 24;
		graphics_->h_adjustment_ = 10;
	}
	else
	{
		graphics_->current_frame_x_ = 0;
		graphics_->h_adjustment_ = 7;
	}
};

void PolloSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	PolloDelAireSprite::polloInteract( my_collision, their_collision, them, level_state.health(), level_state.events(), level_state.inventory(), *this );
};
