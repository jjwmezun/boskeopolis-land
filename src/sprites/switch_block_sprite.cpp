#include "audio.hpp"
#include "event_system.hpp"
#include "sprite_graphics.hpp"
#include "switch_block_sprite.hpp"

SwitchBlockSprite::SwitchBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 0, 112 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

SwitchBlockSprite::~SwitchBlockSprite() {};

void SwitchBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	bump_under_block_component.update( *this );
	graphics_->current_frame_x_ = ( events.switchOn() ) ? 0 : 16;
};

void SwitchBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	const bool hit = bump_under_block_component.testHit( *this, them, their_collision );
	if ( hit )
	{
		events.flipSwitch();
		Audio::playSound( Audio::SoundType::SWITCH );
	}
};
