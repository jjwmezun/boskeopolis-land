#include <algorithm>
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "locked_door_sprite.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"

LockedDoorSprite::LockedDoorSprite( int x, int y, int map_id )
:
	Sprite( nullptr, x, y, 16, 16 * 6, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING ),
	has_key_ ( false ),
	opening_ ( false )
{};

LockedDoorSprite::~LockedDoorSprite() {};

void LockedDoorSprite::customUpdate( LevelState& level_state )
{
	EventSystem& events = level_state.events();
	has_key_ = events.hasKey();

	if ( events.trainDoorPartlyOpen() && hit_box_.h >= ( 16000 * 6 ) )
	{
		killNoAnimation();
		death_finished_ = true;
	}

	if ( opening_ )
	{
		hit_box_.h = std::max( 0, hit_box_.h - 2000 );
		events.setOpening();
	}

	if ( hit_box_.h == 0 )
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
		block_interact_ = false;
	}

	if ( fellInBottomlessPit( level_state.currentMap() ) )
	{
		events.setOpen();
	}
};

void LockedDoorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}

	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideAny() && has_key_ )
		{
			opening_ = true;
		}
	}
};

void LockedDoorSprite::render( Camera& camera )
{
	for ( int y = yPixels(); y < bottomPixels(); y += 2 )
	{
		Render::renderObject( "sprites/locked_door.png", { 0, 0, 16, 2 }, { xPixels(), y, 16, 2 }, SDL_FLIP_NONE, 0, 255, &camera );
	}

	Render::renderObject( "sprites/locked_door.png", { 0, 2, 16, 12 }, { centerXPixels() - 8, std::max( yPixels(), centerYPixels() - 6 ), 16, 12 }, SDL_FLIP_NONE, 0, 255, &camera );
};
