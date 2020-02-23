#include "level_state.hpp"
#include "main.hpp"
#include "olive_spawner_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

static constexpr int SPAWN_SPEED = 300;

OliveSpawnerSprite::OliveSpawnerSprite( int x, int y, Direction::Horizontal start_dir )
:
	Sprite( nullptr, x, y, 16, 16, { SpriteType::ENEMY }, 0, 0, 0, 0, start_dir, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, false )
{};

OliveSpawnerSprite::~OliveSpawnerSprite() {};

void OliveSpawnerSprite::customUpdate( LevelState& level_state )
{
	if ( Main::stateFrame() % SPAWN_SPEED == 0 )
	{
		level_state.sprites().spawnOlive( xPixels() + 4, yPixels(), direction_x_ );
	}
};

void OliveSpawnerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};