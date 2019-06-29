#include "mezun_math.hpp"
#include "swamp_monster_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static int getJumpSpeed()
{
	return mezun::randInt( 9, 6 ) * 1000;
}

static int getTopSpeed()
{
	return mezun::randInt( 1, 3 ) * 500;
}

SwampMonsterSprite::SwampMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/swamp-monster.png" ), x, y, 16, 32, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 50, 1000, getTopSpeed(), getJumpSpeed(), Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, true )
{
	jump_lock_ = true;
};

SwampMonsterSprite::~SwampMonsterSprite() {};

void SwampMonsterSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( !jump_lock_ )
	{
		jump();
		moveLeft();
	}

	if ( vy_ == -jump_top_speed_ )
	{
		block_interact_ = false;
	}
};

void SwampMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::RAFT ) )
	{
		if ( hit_box_.x < them.hit_box_.x + 96000 )
		{
			jump_lock_ = false;
		}
	}
};
