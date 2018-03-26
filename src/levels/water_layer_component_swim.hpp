#pragma once

#include "water_layer_component.hpp"

class WaterLayerComponentSwim : public WaterLayerComponent
{
	public:
		WaterLayerComponentSwim();
		void interact( MapLayerWater& water, Sprite& sprite, Health& health ) override;
};