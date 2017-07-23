#include "camera.h"
#include "event_system.h"
#include "sprite.h"
#include "map_layer_water.h"

MapLayerWater::MapLayerWater( int y_blocks )
:
	MapLayer(),
	y_ ( Unit::BlocksToSubPixels( y_blocks ) ),
	surface_
	(
		"bg/animated_water.png",
		{
			std::make_pair<int, int> ( 0, 0 ),
			std::make_pair<int, int> ( 8, 0 ),
			std::make_pair<int, int> ( 16, 0 ),
			std::make_pair<int, int> ( 24, 0 ),
		}
	),
	x_offset_ ( -8, 0, -8, true ),
	animation_speed_ ( 16 )
{};

void MapLayerWater::interact( Sprite& sprite )
{
	if ( sprite.centerYSubPixels() > y_ )
	{
		sprite.submerged_in_water_ = true;
	}

	if ( sprite.bottomSubPixels() > y_ )
	{
		sprite.swim();
	}
};

void MapLayerWater::render( Camera& camera ) const
{
	if ( Unit::SubPixelsToPixels( y_ ) < camera.bottom() )
	{
		int relative_y = Unit::SubPixelsToPixels( y_ ) - camera.y();

		for ( int x = x_offset_(); x < Unit::WINDOW_WIDTH_PIXELS; x += 8 )
		{
			const sdl2::SDLRect r = { x, relative_y, 8, 16 };
			surface_.render( r, nullptr, false, ALPHA );
		}
		
		relative_y += 16;		
		Render::renderRect( { 0, relative_y, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_WIDTH_PIXELS - relative_y }, COLOR, ALPHA );
	}
};

void MapLayerWater::update( EventSystem& events, BlockSystem& blocks, const Camera& camera )
{
	surface_.update();

	if ( animation_speed_.hit() )
	{
		++x_offset_;
	}

	animation_speed_.update();

	if ( events.waterShouldMove() )
	{
		if ( events.move_water_ < y_ )
		{
			y_ -= MOVE_SPEED;
		}
		else if ( events.move_water_ > y_ )
		{
			y_ += MOVE_SPEED;
		}
	}

	events.current_water_ = y_;
};