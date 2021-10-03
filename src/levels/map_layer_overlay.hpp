#pragma once

#include "map_layer.hpp"
#include <SDL2/SDL.h>

class MapLayerOverlay final : public MapLayer
{
	public:
		enum class OverlayType
		{
			NORMAL,
			MULTIPLY,
			LIGHTEN
		};
		MapLayerOverlay( Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255, OverlayType type = OverlayType::NORMAL, Unit::Layer layer_position = DEFAULT_POSITION );
		~MapLayerOverlay();
		void render( const Camera& camera ) override;
		void update( LevelState& level_state ) override;

	private:
		Uint8 red_;
		Uint8 green_;
		Uint8 blue_;
		Uint8 alpha_;
		OverlayType type_;
};
