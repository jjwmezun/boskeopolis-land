#include "dungeon_enemy_walls_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

DungeonEnemyWallsSprite::DungeonEnemyWallsSprite( int x, int y, Direction::Simple direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/dungeon2.png", 96, 32 ), x, y, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{
	direction_ = direction;
	switch ( direction )
	{
		case ( Direction::Simple::LEFT ):
		{
		}
		break;

		case ( Direction::Simple::UP ):
		{
			graphics_->current_frame_x_ = 144;
			hit_box_.w = 48000;
		}
		break;
	}
};

DungeonEnemyWallsSprite::~DungeonEnemyWallsSprite() {};

void DungeonEnemyWallsSprite::customUpdate( LevelState& level_state )
{
	if ( !level_state.sprites().spritesOnScreen( level_state.camera(), SpriteType::ENEMY ) )
	{
		switch ( direction_ )
		{
			case ( Direction::Simple::LEFT ):
			{
				if ( hit_box_.y > original_hit_box_.y - hit_box_.h )
				{
					hit_box_.y -= 250;
				}
			}
			break;

			case ( Direction::Simple::UP ):
			{
				if ( hit_box_.x < original_hit_box_.x + hit_box_.w )
				{
					hit_box_.x += 250;
				}
			}
			break;
		}
	}
};

void DungeonEnemyWallsSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
	}
};
