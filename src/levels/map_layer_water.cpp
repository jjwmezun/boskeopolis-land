#include "camera.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "sprite.hpp"
#include "map_layer_water.hpp"

#include <iostream>

static constexpr int WATER_MOVE_MAX = 900;
static constexpr int WATER_MOVE_MIN = 650;

MapLayerWater::MapLayerWater( int y_blocks, bool rising )
:
	MapLayer(),
	y_ ( Unit::BlocksToSubPixels( y_blocks ) ),
	rising_ ( rising ),
	surface_
	(
		"bg/animated_water.png",
		{
			std::make_pair<int, int> ( 0, 0 ),
			std::make_pair<int, int> ( 8, 0 ),
			std::make_pair<int, int> ( 16, 0 ),
			std::make_pair<int, int> ( 24, 0 ),
		},
		false,
		false,
		0,
		false,
		0,
		0,
		0,
		0,
		8,
		ALPHA
	),
	x_offset_ ( -8, 0, -8, true ),
	animation_speed_ ( 16 ),
	move_speed_ ( 700 )
{};

void MapLayerWater::interact( Sprite& sprite, Health& health )
{
	if ( sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( sprite.centerYSubPixels() > y_ )
		{
			health.submerge();
		}

		if ( rising_ )
		{
			const int extra_speed = Unit::SubPixelsToPixels( y_ - sprite.hit_box_.y );
			move_speed_ = std::max( std::min( WATER_MOVE_MIN + extra_speed, WATER_MOVE_MAX ), WATER_MOVE_MIN );
		}
	}

	if ( sprite.bottomSubPixels() > y_ + 8000 )
	{
		sprite.in_water_ = true;
	}
};

void MapLayerWater::render( const Camera& camera ) const
{
	if ( Unit::SubPixelsToPixels( y_ ) < camera.bottom() )
	{
		int relative_y = Unit::SubPixelsToPixels( y_ ) - camera.y();

		for ( int x = x_offset_(); x < Unit::WINDOW_WIDTH_PIXELS; x += 8 )
		{
			const sdl2::SDLRect r = { x, relative_y, 8, 16 };
			surface_.render( r, nullptr, false );
		}
		
		relative_y += 16;		
		Render::renderRect( { 0, relative_y, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_WIDTH_PIXELS - relative_y }, COLOR, ALPHA );
	}
};

void MapLayerWater::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap )
{
	surface_.update();

	if ( animation_speed_.hit() )
	{
		++x_offset_;
	}

	animation_speed_.update();

	if ( rising_ )
	{
		y_ -= move_speed_;
	}
	else if ( events.waterShouldMove() )
	{
		if ( events.move_water_ < y_ )
		{
			y_ -= move_speed_;
		}
		else if ( events.move_water_ > y_ )
		{
			y_ += move_speed_;
		}
	}

	events.current_water_ = y_;
};