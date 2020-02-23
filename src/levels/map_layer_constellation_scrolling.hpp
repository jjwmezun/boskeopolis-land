#pragma once

#include "map_layer.hpp"
#include "mezun_sdl2.hpp"

class MapLayerConstellationScrolling : public MapLayer
{
	public:
		MapLayerConstellationScrolling
		(
			int map_width_blocks,
			int map_height_blocks
		);
		~MapLayerConstellationScrolling();
		void update( LevelState& level_state ) override;
		void render( const Camera& camera ) override;

	private:
		const sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
		const double scroll_speed_;
		SDL_Texture* texture_;
};
