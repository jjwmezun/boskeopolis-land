#include "falling_snow_boulder_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "unit.hpp"

static constexpr int BOTTOM = Unit::BlocksToSubPixels( 35 ) + Unit::PixelsToSubPixels( 4 );

FallingSnowBoulderSprite::FallingSnowBoulderSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/snowboulder.png", 0, 0, false, false, 0, true, -4, -4, 8, 8 ), x, y, 24, 24, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, true, false ),
	timer_ ( 0 )
{};

FallingSnowBoulderSprite::~FallingSnowBoulderSprite() {};

void FallingSnowBoulderSprite::customUpdate( LevelState& level_state )
{
	if ( fellInBottomlessPit( level_state.currentMap() ) )
	{
		killNoAnimation();
	}

	if ( hit_box_.y >= BOTTOM )
	{
		hit_box_.y = BOTTOM;
		sprite_interact_ = false;
		kill();
	}
};

void FallingSnowBoulderSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		level_state.health().hurt();
	}
};

void FallingSnowBoulderSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	fullStopY();
	hit_box_.y = BOTTOM;
	if ( timer_ >= 3 )
	{
		if ( graphics_->current_frame_x_ >= 128 )
		{
			death_finished_ = true;
		}
		else
		{
			graphics_->current_frame_x_ += 32;
			timer_ = 0;
		}
	}
	++timer_;
};
