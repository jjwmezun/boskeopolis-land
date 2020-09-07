#include "audio.hpp"
#include "collision.hpp"
#include "spring_sprite.hpp"
#include "sprite_graphics.hpp"

SpringSprite::SpringSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/spring.png", 0, 0 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING )
{};

SpringSprite::~SpringSprite() {};

void SpringSprite::customUpdate( LevelState& level_state )
{
	if ( hit_box_.h < 16000 )
	{
		hit_box_.h += 1000;
	}

	// In case o' wind.
	hit_box_.x = original_hit_box_.x;
	hit_box_.y = original_hit_box_.y + ( 16000 - hit_box_.h );
	
	graphics_->current_frame_x_ = floor( ( 16000 - hit_box_.h ) / 1000 ) * 16;
};

void SpringSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.collideBottomOnly( their_collision, *this ) )
	{
		them.bounce( STRENGTH );
		const int press_down_amount = ( them.vy_ * 10 ) - ( them.vy_ % 1000 );
		hit_box_.h = 8000;
	}
	else if ( their_collision.collideLeft() )
	{
		them.bounceLeft( their_collision.overlapXLeft() );
	}
	else if ( their_collision.collideRight() )
	{
		them.bounceRight( their_collision.overlapXRight() );
	}
	else if ( their_collision.collideTop() )
	{
		them.bounceDownward( their_collision.overlapYTop() );
	}

	if ( their_collision.collideAny() && them.hasType( SpriteType::HERO ) )
	{
		Audio::playSound( Audio::SoundType::BOUNCE );
	}

	printf( "%d\n", hit_box_.h );
};