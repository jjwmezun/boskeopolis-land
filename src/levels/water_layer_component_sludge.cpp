#include "health.hpp"
#include "sprite.hpp"
#include "map_layer_water.hpp"
#include "water_layer_component_sludge.hpp"

WaterLayerComponentSludge::WaterLayerComponentSludge() : submerged_ ( 0 ) {};

void WaterLayerComponentSludge::interact( MapLayerWater& water, Sprite& sprite, Health& health )
{
	if ( sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( sprite.centerYSubPixels() > water.y_ )
		{
			submerged_ = 4;
		}
		else if ( sprite.on_ground_prev_ )
		{
			submerged_ = -1;
		}
		else
		{
			submerged_ = 0;
		}
	}

	if ( sprite.bottomSubPixels() > water.y_ + Unit::PixelsToSubPixels( 8 ) )
	{
		sprite.in_water_ = true;
	}

	health.submerge( submerged_ );
};