#pragma once

#include "water_layer_component.hpp"

class MapLayerWater;

class MapLayerWaterBack : public MapLayer
{
	public:
		MapLayerWaterBack( MapLayerWater* water_ptr );
		~MapLayerWaterBack();
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) override;
		void render( const Camera& camera ) override;
		
	private:
		sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
		MapLayerWater* water_ptr_;
};