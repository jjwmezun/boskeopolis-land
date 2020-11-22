#include "camera.hpp"
#include "map_layer_water.hpp"
#include "map_layer_water_back.hpp"
#include "unit.hpp"

static constexpr int HEIGHT = 6;

MapLayerWaterBack::MapLayerWaterBack( MapLayerWater* water_ptr )
:
	src_ ( 0, 0, MapLayerWater::WIDTH, HEIGHT ),
	dest_ ( 0, MapLayerWater::MAX_FRAME, MapLayerWater::WIDTH, HEIGHT ),
	water_ptr_ ( water_ptr )
{
	layer_position_ = Unit::Layer::BEFORE_BG_1;
};

MapLayerWaterBack::~MapLayerWaterBack() {};

void MapLayerWaterBack::render( const Camera& camera )
{
	if ( dest_.y < camera.bottom() )
	{
		Render::renderObject( water_ptr_->texture_, src_, dest_ );
	}
};

void MapLayerWaterBack::update( LevelState& level_state )
{
	src_.y = MapLayerWater::MAX_FRAME + ( ( water_ptr_->src_.y / MapLayerWater::HEIGHT ) * HEIGHT );
	dest_.y = water_ptr_->dest_.y - 1;
	dest_.x = water_ptr_->dest_.x;
};
