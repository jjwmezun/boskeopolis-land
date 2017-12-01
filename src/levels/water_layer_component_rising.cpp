#include "water_layer_component_rising.hpp"
#include "sprite.hpp"
#include "map_layer_water.hpp"

static constexpr int WATER_MOVE_DEFAULT = 700;
static constexpr int WATER_MOVE_MAX = 900;
static constexpr int WATER_MOVE_MIN = 650;

WaterLayerComponentRising::WaterLayerComponentRising() : move_speed_ ( WATER_MOVE_DEFAULT ) {};

void WaterLayerComponentRising::update( MapLayerWater& water )
{
	water.y_ -= move_speed_;
};

void WaterLayerComponentRising::interact( MapLayerWater& water, Sprite& sprite, Health& health )
{
	const int extra_speed = Unit::SubPixelsToPixels( water.y_ - sprite.hit_box_.y );
	move_speed_ = std::max( std::min( WATER_MOVE_MIN + extra_speed, WATER_MOVE_MAX ), WATER_MOVE_MIN );
};