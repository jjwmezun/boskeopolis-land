#include "camera.hpp"
#include "collision.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "moon_sprite.hpp"
#include "sprite_graphics.hpp"

#include <iostream>

static constexpr int TIME_LIMIT = 45;

MoonSprite::MoonSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/moon.png" ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	moon_state_ ( MoonState::UNUSED )
{};

MoonSprite::~MoonSprite() {};

void MoonSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( moon_state_ )
	{
		case ( MoonState::STARTING ):
			camera.startShaking();
			events.switch_ = true;
			Inventory::clock().reset( Direction::Vertical::DOWN, TIME_LIMIT );
			graphics_->visible_ = false;
			moon_state_ = MoonState::RUNNING;
		break;

		case ( MoonState::RUNNING ):
			if ( Inventory::clock().totalSeconds() >= TIME_LIMIT )
			{
				events.fail();
			}
		break;
	}
};

void MoonSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( moon_state_ == MoonState::UNUSED && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		moon_state_ = MoonState::STARTING;
	}
};