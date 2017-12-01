#pragma once

#include "water_layer_component.hpp"

class WaterLayerComponentSludge : public WaterLayerComponent
{
	public:
		WaterLayerComponentSludge();
		void interact( MapLayerWater& water, Sprite& sprite, Health& health ) override;

	private:
		int submerged_;
};