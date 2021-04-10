#include "audio.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "light_switch_sprite.hpp"
#include "sprite_graphics.hpp"

LightSwitchSprite::LightSwitchSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/light-switch.png", 0, 0, false, false, 0.0, 0, 0, 0, 0, 255 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{
	layer_ = Unit::Layer::AFTER_FG_2;
};

LightSwitchSprite::~LightSwitchSprite() {};

void LightSwitchSprite::customUpdate( LevelState& level_state )
{
	bump_under_block_component.update( *this );
};

void LightSwitchSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	const Collision collision = them.testBlockCollision( *this );
	const bool hit = bump_under_block_component.testHit( *this, them, collision );
	if ( hit )
	{
		Audio::playSound( Audio::SoundType::SWITCH );
        level_state.events().forceSwitchOn();
	}
};

void LightSwitchSprite::render( const Camera& camera ) const
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
};
