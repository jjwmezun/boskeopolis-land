#include "dungeon_enemy_walls_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

DungeonEnemyWallsSprite::DungeonEnemyWallsSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/dungeon2.png", 96, 32 ), x, y, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

DungeonEnemyWallsSprite::~DungeonEnemyWallsSprite() {};

void DungeonEnemyWallsSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void DungeonEnemyWallsSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};
