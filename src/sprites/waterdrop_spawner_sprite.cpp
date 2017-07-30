#include "collision.hpp"
#include "health.hpp"
#include "waterdrop_spawner_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include <iostream>

WaterdropSpawnerSprite::WaterdropSpawnerSprite( int x, int y )
:
	Sprite( nullptr, x + 2, y + 16, 12, 14, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, false ),
	timer_ ( 1 )
{
};

WaterdropSpawnerSprite::~WaterdropSpawnerSprite() {};

void WaterdropSpawnerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( timer_ % 120 == 0 )
	{
		sprites.spawnWaterdrop( xPixels(), yPixels() );
	}
	
	++timer_;
};

void WaterdropSpawnerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
};