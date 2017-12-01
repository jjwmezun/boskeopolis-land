#include "water_layer_component_swim.hpp"
#include "health.hpp"
#include "sprite.hpp"
#include "map_layer_water.hpp"

WaterLayerComponentSwim::WaterLayerComponentSwim() {};

void WaterLayerComponentSwim::interact( MapLayerWater& water, Sprite& sprite, Health& health )
{
	if ( sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( sprite.centerYSubPixels() > water.y_ )
		{
			health.submerge();
		}
	}

	if ( sprite.bottomSubPixels() > water.y_ + Unit::PixelsToSubPixels( 8 ) )
	{
		sprite.in_water_ = true;
	}
};