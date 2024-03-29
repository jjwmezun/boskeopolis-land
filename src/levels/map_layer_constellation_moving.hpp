#pragma once

#include "map_layer.hpp"
#include "mezun_sdl2.hpp"

class MapLayerConstellationMoving final : public MapLayer
{
	public:
		MapLayerConstellationMoving( int width, int height, double move_speed, std::string image, Unit::Layer layer_position = DEFAULT_POSITION );
		~MapLayerConstellationMoving();
		void update( LevelState& level_state ) override;
		void render( const Camera& camera ) override;

	private:
		const sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
		sdl2::SDLRect dest2_;
		std::string image_;
		const double move_speed_;
		SDL_Texture* texture_;
};
