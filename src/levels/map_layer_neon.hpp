#pragma once

#include "map_layer.hpp"
#include <SDL2/SDL.h>

class MapLayerNeon : public MapLayer
{
	public:
		MapLayerNeon();
		~MapLayerNeon();
		void render( const Camera& camera ) override;
		void update( LevelState& level_state ) override;

	private:
		SDL_Color color_;
		int hue_;

		void convertHSVToRGB();
};
