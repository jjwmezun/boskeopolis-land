#include "collision.h"
#include "lil_pipe_monster_sprite.h"
#include "sprite_graphics.h"

LilPipeMonsterSprite::LilPipeMonsterSprite( int x, int y, Direction::Vertical direction )
:
	Sprite
	(
		std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 16, 16, false, direction == Direction::Vertical::DOWN, 0, false, -3, -3, 6, 6 ),
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

void LilPipeMonsterSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
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

void LilPipeMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
};