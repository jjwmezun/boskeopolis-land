#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "pollo_del_aire_vertical_sprite.hpp"
#include "sprite_graphics.hpp"

PolloDelAireVerticalSprite::PolloDelAireVerticalSprite( int x, int y, Direction::Vertical dir_y, int map_id, bool despawn_when_dead )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo_no_noko.png", 0, 32, false, false, 0, false, -1, -6, 2, 8 ), x, y, 22, 22, { SpriteType::DEATH_COUNT }, 500, 1000, 500, 4000, Direction::Horizontal::LEFT, dir_y, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, despawn_when_dead, true, true, .2, map_id ),
	animation_counter_ ( 0 )
{};

PolloDelAireVerticalSprite::~PolloDelAireVerticalSprite() {};

void PolloDelAireVerticalSprite::deathAction( LevelState& level_state )
{
	PolloDelAireSprite::polloDeath( level_state.camera(), *this, level_state.currentMap() );
};

void PolloDelAireVerticalSprite::reset()
{
	PolloDelAireSprite::polloReset( *this );
	graphics_->current_frame_y_ = 32;
};

void PolloDelAireVerticalSprite::customUpdate( LevelState& level_state )
{
	if ( direction_y_ == Direction::Vertical::UP )
	{
		moveUp();
	}
	else
	{
		moveDown();
	}

	if ( hit_box_.y < original_hit_box_.y - ( 5 * 16000 ) || collide_bottom_ )
	{
		direction_y_ = Direction::Vertical::DOWN;
	}
	else if ( hit_box_.y > original_hit_box_.y + ( 5 * 16000 ) || collide_top_ )
	{
		direction_y_ = Direction::Vertical::UP;
	}

	++animation_counter_;
	if ( animation_counter_ >= 16 ) animation_counter_ = 0;
	graphics_->current_frame_x_ = 24 * floor(animation_counter_ / 8 );
};

void PolloDelAireVerticalSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	PolloDelAireSprite::polloInteract( my_collision, their_collision, them, level_state.health(), level_state.events(), level_state.inventory(), *this );
};
