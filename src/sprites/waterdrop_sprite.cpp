#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "waterdrop_sprite.hpp"
#include "waterdrop_graphics.hpp"

WaterdropSprite::WaterdropSprite( int x, int y )
:
	Sprite( std::make_unique<WaterdropGraphics> (), x, y, 14, 16, {}, 100, 1500, 400, 2400, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true )
{};

WaterdropSprite::~WaterdropSprite() {};

void WaterdropSprite::customUpdate( LevelState& level_state )
{
	moveDown();
};

void WaterdropSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
		if ( them.hasType( SpriteType::HERO ) && !is_dead_ )
		{
			level_state.health().hurt();
		}

		kill();
	}
};

void WaterdropSprite::deathAction( LevelState& level_state )
{
	fullStopY();

	if ( graphics_->current_frame_x_ >= 5 * 14 )
	{
		death_finished_ = true;
	}
};
