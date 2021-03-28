#pragma once

#include "map_layer_rain.hpp"
#include <vector>

class MapLayerRainGradual : public MapLayer
{
	public:
		MapLayerRainGradual( int start, int end, Unit::Layer layer_position = DEFAULT_POSITION );
		~MapLayerRainGradual();
		void update( LevelState& level_state ) override;
		void render( const Camera& camera ) override;

    private:
        int start_;
        int max_;
        MapLayerRain layer_;
};
