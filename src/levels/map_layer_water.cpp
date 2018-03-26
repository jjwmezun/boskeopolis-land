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
	return new MapLayerWater( y_blocks, { new WaterLayerComponentSludge() }, "bg/animated_water_3.png", 255, 6 );
};

MapLayerWater::MapLayerWater( int y_blocks, ComponentGroup components, std::string gfx, Uint8 alpha, int color )
:
	MapLayer(),
	src_ ( 0, 0, WIDTH, HEIGHT ),
	dest_ ( 0, 0, WIDTH, HEIGHT ),
	body_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	components_ ( components ),
	texture_ ( gfx ),
	y_ ( Unit::BlocksToSubPixels( y_blocks ) ),
	color_ ( color ),
	x_offset_ ( 0 ),
	timer_ ( 0 ),
	alpha_ ( alpha )
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
	if ( dest_.y < camera.bottom() )
	{
		Render::renderObject( texture_, src_, dest_, false, false, 0.0, alpha_, nullptr );
		Render::renderRect( body_, color_, alpha_ );
	}
};

void MapLayerWater::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap )
{
	updateGFX( camera );
	updateComponents();
	updateEventMovement( events );
};

void MapLayerWater::updateGFX( const Camera& camera )
{
	if ( timer_ == 3 || timer_ == 7 )
	{
		--x_offset_;
		if ( x_offset_ <= 8 ) { x_offset_ = 0; }
		dest_.x = x_offset_;
	}
	
	if ( timer_ == 8 )
	{
		src_.y += HEIGHT;
		if ( src_.y >= MAX_FRAME ) { src_.y = 0; }
		timer_ = 0;
	}
	else
	{
		++timer_;
	}
	dest_.y = Unit::SubPixelsToPixels( y_ ) - camera.y();
	body_.y = dest_.y + HEIGHT;
	body_.h = Unit::WINDOW_HEIGHT_PIXELS - body_.y;
};

void MapLayerWater::updateComponents()
{
	for ( auto* c : components_ )
	{
		c->update( *this );
	}
};

void MapLayerWater::updateEventMovement( EventSystem& events )
{
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