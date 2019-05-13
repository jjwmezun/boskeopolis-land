#pragma once

#include "map_layer.hpp"
#include "mezun_sdl2.hpp"

class MapLayerConstellationMoving : public MapLayer
{
	public:
		MapLayerConstellationMoving( int width, int height, double move_speed );
		~MapLayerConstellationMoving();
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites ) override;
		void render( const Camera& camera ) override;

	private:
		const sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
		sdl2::SDLRect dest2_;
		const double move_speed_;
		SDL_Texture* texture_;
};
