#pragma once

#include "water_layer_component.hpp"

class WaterLayerComponentSludge final : public WaterLayerComponent
{
	public:
		WaterLayerComponentSludge();
		void interact( MapLayerWater& water, Sprite& sprite, Health& health ) override;

	private:
		int submerged_;
};