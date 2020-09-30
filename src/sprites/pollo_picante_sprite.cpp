#include "collision.hpp"
#include "level_state.hpp"
#include <memory>
#include "pollo_del_aire_sprite.hpp"
#include "pollo_flame_sprite.hpp"
#include "pollo_picante_sprite.hpp"
#include "sprite_graphics.hpp"

PolloPicanteSprite::PolloPicanteSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo-picante.png", 0, 0, direction == Direction::Horizontal::RIGHT, false, 0, -1, -6, 2, 8 ), x, y, 22, 22, { SpriteType::DEATH_COUNT }, 250, 500, 500, 4000, direction, Direction::Vertical::UP, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false ),
	animation_counter_ ( 0 ),
    shoot_timer_ ( 0 )
{};

PolloPicanteSprite::~PolloPicanteSprite() {};

void PolloPicanteSprite::deathAction( LevelState& level_state )
{
	PolloDelAireSprite::polloDeath( level_state.camera(), *this, level_state.currentMap() );
	graphics_->current_frame_y_ = 4;
};

void PolloPicanteSprite::reset()
{
	PolloDelAireSprite::polloReset( *this );
    graphics_->current_frame_y_ = 0;
};

void PolloPicanteSprite::customUpdate( LevelState& level_state )
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

    if ( shoot_timer_ == 60 )
    {
        shoot_timer_ = 0;
        level_state.sprites().spawn( std::unique_ptr<PolloFlameSprite> ( new PolloFlameSprite( centerXPixels(), centerYPixels(), direction_x_ ) ) );
    }
    else
    {
        ++shoot_timer_;
    }

	++animation_counter_;
	if ( animation_counter_ >= 16 ) animation_counter_ = 0;
	graphics_->current_frame_x_ = 24 * floor( animation_counter_ / 8 );
};

void PolloPicanteSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	PolloDelAireSprite::polloInteract( my_collision, their_collision, them, level_state.health(), level_state.events(), level_state.inventory(), *this );
};
