#pragma once

#include "map_layer.hpp"
#include <SDL2/SDL.h>

class MapLayerOverlayTransition final : public MapLayer
{
	public:
		MapLayerOverlayTransition( Palette from, Palette to, int start, int end, Unit::Layer layer_position = DEFAULT_POSITION );
		~MapLayerOverlayTransition();
		void render( const Camera& camera ) override;
		void update( LevelState& level_state ) override;

	private:
		Uint8 red_;
		Uint8 green_;
		Uint8 blue_;
		Uint8 alpha_;
        int max_;
        int start_;
        int from_r;
        int from_g;
        int from_b;
        int from_a;
        double diff_r;
        double diff_g;
        double diff_b;
        double diff_a;
        Palette from_;
        Palette to_;
        bool set_;
        char scratch_[ 3000 ];
};
