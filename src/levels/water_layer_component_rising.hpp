#pragma once

#include "water_layer_component.hpp"

class WaterLayerComponentRising final : public WaterLayerComponent
{
	public:
		WaterLayerComponentRising();
		void update( MapLayerWater& water ) override;
		void interact( MapLayerWater& water, Sprite& sprite, Health& health ) override;

	private:
		int move_speed_;
};