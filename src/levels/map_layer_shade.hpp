#pragma once

#include "map_layer.hpp"
#include <SDL2/SDL.h>

class MapLayerShade : public MapLayer
{
	public:
		MapLayerShade( int shade, int alpha );
		~MapLayerShade();
		void render( const Camera& camera ) const override;
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) override;

	private:
		int shade_;
		Uint8 alpha_;
};