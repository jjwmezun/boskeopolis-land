#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "pollo_del_aire_still_sprite.hpp"
#include "sprite_graphics.hpp"

PolloDelAireStillSprite::PolloDelAireStillSprite( int x, int y, int map_id, bool despawn_when_dead )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo_no_noko.png", 0, 32, false, false, 0, -1, -6, 2, 8 ), x, y, 22, 22, { SpriteType::DEATH_COUNT }, 250, 500, 500, 4000, Direction::Horizontal::__NULL, Direction::Vertical::UP, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, despawn_when_dead, true, true, .2, map_id ),
	animation_counter_ ( 0 )
{};

PolloDelAireStillSprite::~PolloDelAireStillSprite() {};

void PolloDelAireStillSprite::deathAction( LevelState& level_state )
{
	PolloDelAireSprite::polloDeath( level_state.camera(), *this, level_state.currentMap() );
};

void PolloDelAireStillSprite::reset()
{
	PolloDelAireSprite::polloReset( *this );
};

void PolloDelAireStillSprite::customUpdate( LevelState& level_state )
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

void PolloDelAireStillSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	PolloDelAireSprite::polloInteract( my_collision, their_collision, them, level_state.health(), level_state.events(), level_state.inventory(), *this );
};
