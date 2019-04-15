#include "collision.hpp"
#include "dungeon_switch_sprite.hpp"
#include "event_system.hpp"
#include "sprite_graphics.hpp"

DungeonSwitchSprite::DungeonSwitchSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/dungeon2.png", 0, 48 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	on_ ( false )
{};

DungeonSwitchSprite::~DungeonSwitchSprite() {};

void DungeonSwitchSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	graphics_->flip_x_ = on_;
};

void DungeonSwitchSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( !on_ )
	{
		if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
		{
			events.flipSwitch();
			on_ = true;
		}
	}
};
