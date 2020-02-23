#include "collision.hpp"
#include "dungeon_switch_sprite.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

DungeonSwitchSprite::DungeonSwitchSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/dungeon2.png", 0, 48 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	on_ ( false )
{};

DungeonSwitchSprite::~DungeonSwitchSprite() {};

void DungeonSwitchSprite::customUpdate( LevelState& level_state )
{
	graphics_->flip_x_ = on_;
};

void DungeonSwitchSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( !on_ )
	{
		if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
		{
			level_state.events().flipSwitch();
			on_ = true;
		}
	}
};
