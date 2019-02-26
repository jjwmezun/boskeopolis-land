#pragma once

#include "block.hpp"
#include "map_layer.hpp"
#include <vector>

// More optimzied way to render tilemap BG.
// 'Stead o' rendering block-by-block, creates 1 texture o' whole map @ the start
// & just renders a portion o' this big texture every frame,
// cutting down # o' RenderCopy calls.
// Downside: can't use animated tiles with this.

class MapLayerTilemapImage : public MapLayer
{
	public:
		MapLayerTilemapImage( const std::vector<int>& tiles, int width, int height );
		~MapLayerTilemapImage();
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) override;
		void render( const Camera& camera ) override;

	private:
		std::vector<int> tiles_;
		SDL_Texture* texture_;
		const sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
		const int width_;
		const int height_;
};
