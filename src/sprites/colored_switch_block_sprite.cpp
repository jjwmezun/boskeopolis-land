#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "colored_switch_block_sprite.hpp"

ColoredSwitchBlockSprite::ColoredSwitchBlockSprite( int x, int y, int value )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/house.png", 176 + Unit::BlocksToPixels( value ), 32 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	value_ ( value )
{};

ColoredSwitchBlockSprite::~ColoredSwitchBlockSprite() {};

void ColoredSwitchBlockSprite::customUpdate( LevelState& level_state )
{
	bump_under_block_component.update( *this );
	graphics_->current_frame_y_ = ( level_state.events().isSwitch( value_ ) ) ? 48 : 32;
};

void ColoredSwitchBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	EventSystem& events = level_state.events();
	if ( !events.isSwitch( value_ ) )
	{
		Collision collision = them.testBlockCollision( *this );
		const bool hit = bump_under_block_component.testHit( *this, them, collision );
		if ( hit && events.switchIsNotLocked( level_state ) )
		{
			events.setSwitchLock( level_state );
			events.forceSwitch( value_ );
			Audio::playSound( Audio::SoundType::SWITCH );
		}
	}
};
