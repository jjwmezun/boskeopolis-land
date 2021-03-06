#include "audio.hpp"
#include "collision.hpp"
#include "bouncy_cloud_block_sprite.hpp"
#include "map.hpp"
#include "sprite_graphics.hpp"

BouncyCloudBlockSprite::BouncyCloudBlockSprite( int x, int y )
:
	Sprite
	(
		std::make_unique<SpriteGraphics>
		(
			"sprites/cloud_block.png",
			64
		),
		x,
		y,
		16,
		16,
		{},
		100,
		100,
		0,
		0,
		Direction::Horizontal::__NULL,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::PERMANENT
	)
{};

BouncyCloudBlockSprite::~BouncyCloudBlockSprite() {};

void BouncyCloudBlockSprite::customUpdate( LevelState& level_state )
{
	if ( hit_box_.y < original_hit_box_.y + 99 || hit_box_.y > original_hit_box_.y - 99 )
	{
		fullStopY();
		hit_box_.y = original_hit_box_.y;
	}
	else if ( hit_box_.y > original_hit_box_.y )
	{
		moveUp();
	}
	else if ( hit_box_.y < original_hit_box_.y )
	{
		moveDown();
	}
	
	// For map loop
	hit_box_.x = original_hit_box_.x;
};

void BouncyCloudBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideBottom() )
		{
			hit_box_.y += them.vy_;	
			them.changeY( hit_box_.y - them.heightSubPixels() );
			them.bounce( ( hit_box_.y - original_hit_box_.y ) / 300 );
			Audio::playSound( Audio::SoundType::BOUNCE );
		}	
	}
};