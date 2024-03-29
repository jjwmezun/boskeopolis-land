#include "collision.hpp"
#include "lil_pipe_monster_sprite.hpp"
#include "sprite_graphics.hpp"

LilPipeMonsterSprite::LilPipeMonsterSprite( int x, int y, Direction::Vertical direction )
:
	Sprite
	(
		std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 16, 16, false, direction == Direction::Vertical::DOWN, 0, -3, -3, 6, 6 ),
		x+3,
		y+3,
		10,
		10,
		{ SpriteType::ENEMY },
		200,
		200,
		0,
		0,
		Direction::Horizontal::__NULL,
		direction,
		nullptr,
		SpriteMovement::Type::FLOATING
	),
	original_direction_y_ ( direction )
{
	block_interact_ = false;
};

LilPipeMonsterSprite::~LilPipeMonsterSprite() {};

void LilPipeMonsterSprite::customUpdate( LevelState& level_state )
{
	switch ( direction_y_ )
	{
		case ( Direction::Vertical::UP ):
			moveUp();
		break;
			
		case ( Direction::Vertical::DOWN ):
			moveDown();
		break;
	}
	
	switch ( original_direction_y_ )
	{
		case ( Direction::Vertical::UP ):
			if ( hit_box_.y < original_hit_box_.y - Unit::BlocksToSubPixels( 1 ) )
			{
				direction_y_ = Direction::Vertical::DOWN;
			}
			else if ( hit_box_.y > original_hit_box_.y )
			{
				direction_y_ = Direction::Vertical::UP;
			}
		break;
			
		case ( Direction::Vertical::DOWN ):
			if ( hit_box_.y < original_hit_box_.y )
			{
				direction_y_ = Direction::Vertical::DOWN;
			}
			else if ( hit_box_.y > original_hit_box_.y + Unit::BlocksToSubPixels( 1 ) )
			{
				direction_y_ = Direction::Vertical::UP;
			}
		break;
	}
};

void LilPipeMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};