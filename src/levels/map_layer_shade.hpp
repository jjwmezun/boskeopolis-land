#pragma once

#include "map_layer.hpp"
#include <SDL2/SDL.h>

class MapLayerShade final : public MapLayer
{
	public:
		MapLayerShade( int shade, int alpha, Unit::Layer layer_position = DEFAULT_POSITION );
		~MapLayerShade();
		void render( const Camera& camera ) override;
		void update( LevelState& level_state ) override;

	private:
		int shade_;
		Uint8 alpha_;
		int timer_;
};
