#include "camera.hpp"
#include <cassert>
#include "event_system.hpp"
#include "map_layer_water.hpp"
#include "water_layer_component_rising.hpp"
#include "water_layer_component_sludge.hpp"
#include "water_layer_component_swim.hpp"

static constexpr int MOVE_SPEED = 700;

MapLayerWater* MapLayerWater::makeNormalWater( int y_blocks )
{
	return new MapLayerWater( y_blocks, { new WaterLayerComponentSwim() } );
};

MapLayerWater* MapLayerWater::makeRisingWater( int y_blocks )
{
	return new MapLayerWater( y_blocks, { new WaterLayerComponentSwim(), new WaterLayerComponentRising() } );
};

MapLayerWater* MapLayerWater::makeSludgeWater( int y_blocks )
{
	return new MapLayerWater( y_blocks, { new WaterLayerComponentSludge() }, "bg/animated_water_2.png", 255, 6 );
};

MapLayerWater::MapLayerWater( int y_blocks, ComponentGroup components, std::string gfx, Uint8 alpha, int color )
:
	MapLayer(),
	components_ ( components ),
	y_ ( Unit::BlocksToSubPixels( y_blocks ) ),
	surface_
	(
		std::move( gfx ),
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
		alpha
	),
	x_offset_ ( -8, 0, -8, true ),
	animation_speed_ ( 16 ),
	color_ ( color )
{};

MapLayerWater::~MapLayerWater()
{
	for ( auto* c : components_ )
	{
		assert( c != nullptr );
		delete c;
	}
};

void MapLayerWater::interact( Sprite& sprite, Health& health )
{
	for ( auto* c : components_ )
	{
		c->interact( *this, sprite, health );
	}
};

void MapLayerWater::render( const Camera& camera )
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
		Render::renderRect( { 0, relative_y, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_WIDTH_PIXELS - relative_y }, color_, surface_.alpha_ );
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

	for ( auto* c : components_ )
	{
		c->update( *this );
	}

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