#include "collision.hpp"
#include "pipe_eel_sprite.hpp"
#include "animated_graphics.hpp"

PipeEelSprite::PipeEelSprite( int x, int y, Direction::Vertical dir_y )
:
	Sprite
	(
		std::make_unique<AnimatedGraphics>
		(
			"sprites/pipe-eel.png",
			std::vector<std::pair<int,int>>
			({
				std::pair<int, int> ( 0, 0 ),
				std::pair<int, int> ( 14, 0 ),
				std::pair<int, int> ( 28, 0 ),
				std::pair<int, int> ( 42, 0 ),
				std::pair<int, int> ( 56, 0 ),
				std::pair<int, int> ( 70, 0 ),
				std::pair<int, int> ( 56, 0 ),
				std::pair<int, int> ( 42, 0 ),
				std::pair<int, int> ( 28, 0 ),
				std::pair<int, int> ( 14, 0 )
			}),
			false,
			( dir_y == Direction::Vertical::UP ),
			0,
			false,
			-1,
			-7,
			2,
			8
		),
		x + 2,
		( ( dir_y == Direction::Vertical::DOWN ) ? y - 38 : y ),
		12,
		38,
		{ SpriteType::ENEMY }, 1000, 1000, 0, 0, Direction::Horizontal::__NULL, dir_y, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false
	)
{
};

PipeEelSprite::~PipeEelSprite() {};

void PipeEelSprite::customUpdate( LevelState& level_state )
{
	switch ( direction_y_ )
	{
		case ( Direction::Vertical::UP ):
			moveUp();

			if ( hit_box_.y < original_hit_box_.y - 128000 )
			{
				hit_box_.y = original_hit_box_.y;
			}
		break;
		
		default:
			moveDown();

			if ( hit_box_.y > original_hit_box_.y + 128000 )
			{
				hit_box_.y = original_hit_box_.y;
			}
		break;
	}
};

void PipeEelSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};