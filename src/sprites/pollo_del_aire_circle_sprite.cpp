#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "pollo_del_aire_sprite.hpp"
#include "pollo_del_aire_circle_sprite.hpp"
#include "sprite_graphics.hpp"

PolloDelAireCircleSprite::PolloDelAireCircleSprite( int x, int y, bool clockwise, int map_id, bool despawn_when_dead )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo_no_noko.png", 0, 32, clockwise, false, 0, false, -1, -6, 2, 8 ), x, y, 22, 22, { SpriteType::DEATH_COUNT }, 500, 1000, 500, 4000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, despawn_when_dead, true, true, .2, map_id ),
	clockwise_ ( clockwise ),
	animation_counter_ ( 0 )
{};

PolloDelAireCircleSprite::~PolloDelAireCircleSprite() {};

void PolloDelAireCircleSprite::deathAction( LevelState& level_state )
{
	PolloDelAireSprite::polloDeath( level_state.camera(), *this, level_state.currentMap() );
};

void PolloDelAireCircleSprite::reset()
{
	PolloDelAireSprite::polloReset( *this );
};

void PolloDelAireCircleSprite::customUpdate( LevelState& level_state )
{
	hit_box_.x = original_hit_box_.x + ( hit_box_.w / 2 ) + ( std::cos( angle() ) * 50000 );
	hit_box_.y = original_hit_box_.y + ( hit_box_.h / 2 ) + ( std::sin( angle() ) * 50000 );

	if ( clockwise_ )
	{
		angle_ += 50;
	}
	else
	{
		angle_ -= 50;
	}

	++animation_counter_;
	if ( animation_counter_ >= 16 ) animation_counter_ = 0;
	graphics_->current_frame_x_ = 24 * floor(animation_counter_ / 8 );
};

void PolloDelAireCircleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	PolloDelAireSprite::polloInteract( my_collision, their_collision, them, level_state.health(), level_state.events(), level_state.inventory(), *this );
};
