#include "falling_snow_boulder_spawn_sprite.hpp"
#include "falling_snow_boulder_sprite.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "sprite_system.hpp"

FallingSnowBoulderSpawnSprite::FallingSnowBoulderSpawnSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/snowboulder.png", 128, 0, false, true, 0 ), x, y, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
	timer_ ( -1 )
{
	layer_ = Unit::Layer::SPRITES_2;
};

FallingSnowBoulderSpawnSprite::~FallingSnowBoulderSpawnSprite() {};

void FallingSnowBoulderSpawnSprite::customUpdate( LevelState& level_state )
{
	if ( timer_ >= 0 )
	{
		if ( timer_ >= 3 )
		{
			if ( graphics_->current_frame_x_ <= 32 )
			{
				level_state.sprites().spawn( std::make_unique<FallingSnowBoulderSprite>( xPixels(), yPixels() ) );
				graphics_->current_frame_x_ = 128;
				timer_ = -1;
				return;
			}
			else
			{
				graphics_->current_frame_x_ -= 32;
				timer_ = 0;
			}
		}
		timer_++;
	}
	else if ( Main::nextStateFrame( 120 ) )
	{
		timer_ = 0;
	}
};

void FallingSnowBoulderSpawnSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) {};
