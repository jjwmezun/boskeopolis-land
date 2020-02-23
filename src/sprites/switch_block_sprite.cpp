#include "audio.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "switch_block_sprite.hpp"

SwitchBlockSprite::SwitchBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 0, 112 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

SwitchBlockSprite::~SwitchBlockSprite() {};

void SwitchBlockSprite::customUpdate( LevelState& level_state )
{
	bump_under_block_component.update( *this );
	graphics_->current_frame_x_ = ( level_state.events().isSwitchOn() ) ? 0 : 16;
};

void SwitchBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const bool hit = bump_under_block_component.testHit( *this, them, their_collision );
	if ( hit )
	{
		level_state.events().flipSwitch();
		Audio::playSound( Audio::SoundType::SWITCH );
	}
};
