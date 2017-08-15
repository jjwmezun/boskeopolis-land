#include "camera.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "sprite.hpp"
#include "map_layer_water.hpp"

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
	animation_speed_ ( 16 )
{};

void MapLayerWater::interact( Sprite& sprite, Health& health )
{
	if ( sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( sprite.centerYSubPixels() > y_ )
		{
			health.submerge();
		}
	}

	if ( sprite.bottomSubPixels() > y_ + 8000 )
	{
		sprite.in_water_ = true;
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
			surface_.render( r, nullptr, false );
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

	if ( rising_ )
	{
		y_ -= MOVE_SPEED;
	}
	else if ( events.waterShouldMove() )
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