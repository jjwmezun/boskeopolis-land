#pragma once

#include "map_layer.hpp"
#include <SDL2/SDL.h>

class MapLayerOverlay final : public MapLayer
{
	public:
		MapLayerOverlay( Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255, Unit::Layer layer_position = DEFAULT_POSITION );
		~MapLayerOverlay();
		void render( const Camera& camera ) override;
		void update( LevelState& level_state ) override;

	private:
		Uint8 red_;
		Uint8 green_;
		Uint8 blue_;
		Uint8 alpha_;
};
