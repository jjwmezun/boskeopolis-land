#include "event_system.hpp"
#include "light_switch_sprite.hpp"
#include "sprite_graphics.hpp"

LightSwitchSprite::LightSwitchSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/light-switch.png", 0, 0, false, false, 0.0, true, 0, 0, 0, 0, 255 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

LightSwitchSprite::~LightSwitchSprite() {};

void LightSwitchSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	bump_under_block_component.update( *this );
};

void LightSwitchSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	const bool hit = bump_under_block_component.testHit( *this, them, their_collision );
	if ( hit )
	{
        events.lightSwitchTurnOn();
	}
};

void LightSwitchSprite::render( Camera& camera, bool priority )
{
};

void LightSwitchSprite::renderSuperPriority( Camera& camera )
{
	graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, true );
};
