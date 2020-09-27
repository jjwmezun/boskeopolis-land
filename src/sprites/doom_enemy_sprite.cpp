#include "audio.hpp"
#include "doom_enemy_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

DoomEnemySprite::DoomEnemySprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, { SpriteType::ENEMY }, 200, 600, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	animation_ ( 0 )
{
	jump_lock_ = false;
};

DoomEnemySprite::~DoomEnemySprite() {};

void DoomEnemySprite::customUpdate( LevelState& level_state )
{};

void DoomEnemySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( !is_dead_ && them.hasType( SpriteType::HERO ) )
	{
		if ( them.rightSubPixels() < hit_box_.x )
		{
			moveLeft();
		}
		else if ( them.hit_box_.x > rightSubPixels() )
		{
			moveRight();
		}

		if ( them.bottomSubPixels() < hit_box_.y )
		{
			moveUp();
		}
		else if ( them.hit_box_.y > bottomSubPixels() )
		{
			moveDown();
		}

		if ( their_collision.collideAny() )
		{
			level_state.health().hurt();
		}
	}
};

void DoomEnemySprite::deathAction( LevelState& level_state )
{
	if ( animation_ == 0 )
	{
		Audio::playSound( Audio::SoundType::HURT );
	}

	if ( animation_ >= 32 )
	{
		death_finished_ = true;
	}
	else
	{
		++animation_;
		jump_lock_ = ( animation_ % 4 == 0 );
	}
};
