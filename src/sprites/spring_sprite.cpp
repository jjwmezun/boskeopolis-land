#include "audio.hpp"
#include "collision.hpp"
#include "spring_sprite.hpp"
#include "sprite_graphics.hpp"

SpringSprite::SpringSprite( int x, int y, int w, int h, std::string tileset )
:
	Sprite( std::make_unique<SpriteGraphics> ( std::move( tileset ), 0, 0 ), x, y + ( 16 - h ), w, h, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING )
{};

SpringSprite::~SpringSprite() {};

void SpringSprite::customUpdate( LevelState& level_state )
{
	if ( hit_box_.h < original_hit_box_.h )
	{
		hit_box_.h += 1000;
	}

	// In case o' wind.
	hit_box_.x = original_hit_box_.x;
	hit_box_.y = original_hit_box_.y + ( original_hit_box_.h - hit_box_.h );
	graphics_->current_frame_y_ = Unit::SubPixelsToPixels( original_hit_box_.h - hit_box_.h );
	graphics_->current_frame_x_ = graphics_->current_frame_y_ * 16;
};

void SpringSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.isUpsideDown() )
	{
		if ( them.collideTopOnly( their_collision, *this ) )
		{
			them.bounce( STRENGTH );
			hit_box_.h = original_hit_box_.h / 2;
			graphics_->current_frame_x_ = graphics_->current_frame_y_ * 16;
			Audio::playSound( Audio::SoundType::BOUNCE );
		}
		else if ( their_collision.collideBottom() )
		{
			them.bounceDownward( their_collision.overlapYTop() );
		}
		else if ( their_collision.collideLeft() )
		{
			them.collideStopXLeft( their_collision.overlapXLeft() );
		}
		else if ( their_collision.collideRight() )
		{
			them.collideStopXRight( their_collision.overlapXRight() );
		}
	}
	else
	{
		if ( them.collideBottomOnly( their_collision, *this ) )
		{
			them.bounce( STRENGTH );
			hit_box_.h = original_hit_box_.h / 2;
			hit_box_.y = original_hit_box_.y + ( original_hit_box_.h - hit_box_.h );
			graphics_->current_frame_y_ = Unit::SubPixelsToPixels( original_hit_box_.h - hit_box_.h );
			graphics_->current_frame_x_ = graphics_->current_frame_y_ * 16;
			Audio::playSound( Audio::SoundType::BOUNCE );
		}
		else if ( their_collision.collideTop() )
		{
			them.bounceDownward( their_collision.overlapYTop() );
		}
		else if ( their_collision.collideLeft() )
		{
			them.collideStopXLeft( their_collision.overlapXLeft() );
		}
		else if ( their_collision.collideRight() )
		{
			them.collideStopXRight( their_collision.overlapXRight() );
		}
	}
};