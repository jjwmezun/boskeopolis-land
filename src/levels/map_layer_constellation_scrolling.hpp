#pragma once

#include "map_layer.hpp"
#include "mezun_sdl2.hpp"

class MapLayerConstellationScrolling final : public MapLayer
{
	public:
		MapLayerConstellationScrolling
		(
			int map_width_blocks,
			int map_height_blocks,
			std::string image,
			int layer_position = DEFAULT_POSITION
		);
		~MapLayerConstellationScrolling();
		void update( LevelState& level_state ) override;
		void render( const Camera& camera ) override;

	private:
		const sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
		std::string image_;
		const double scroll_speed_;
		SDL_Texture* texture_;
};
