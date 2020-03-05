#include "spark_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

SparkSprite::SparkSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/spark.png", 0, 0, false, false, 0.0, false, 0, 0, 0, 4 ), x, y + 12, 0, 4, { SpriteType::ENEMY, SpriteType::SPARK }, 4000, 4000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true ),
	hit_wall_position_ ( -1 )
{};

SparkSprite::~SparkSprite() {};

void SparkSprite::customUpdate( LevelState& level_state )
{
	moveLeft();
	if ( hit_box_.x > original_hit_box_.x - 64000 )
	{
		hit_box_.w = std::min( 64000, original_hit_box_.x - hit_box_.x );
	}
	else if ( hit_wall_position_ > -1 )
	{
		if ( hit_box_.x <= hit_wall_position_ )
		{
			hit_box_.w -= 4000;
			graphics_->current_frame_x_ += 4;
			if ( hit_box_.w <= 0 )
			{
				reset();
			}
		}
	}
	else if ( collide_left_ )
	{
		hit_wall_position_ = hit_box_.x;
	}
};

void SparkSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};

void SparkSprite::reset()
{
	hit_box_ = original_hit_box_;
	graphics_->current_frame_x_ = 0;
};
