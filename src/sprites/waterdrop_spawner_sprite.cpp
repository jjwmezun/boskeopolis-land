#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "waterdrop_spawner_sprite.hpp"

WaterdropSpawnerSprite::WaterdropSpawnerSprite( int x, int y )
:
	Sprite( nullptr, x + 2, y + 16, 12, 14, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, false ),
	timer_ ( 1 )
{
};

WaterdropSpawnerSprite::~WaterdropSpawnerSprite() {};

void WaterdropSpawnerSprite::customUpdate( LevelState& level_state )
{
	if ( timer_ % 120 == 0 )
	{
		level_state.sprites().spawnWaterdrop( xPixels(), yPixels() );
	}
	
	++timer_;
};

void WaterdropSpawnerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};