#pragma once

#include <array>
#include "map_layer.hpp"
#include "mezun_sdl2.hpp"
#include "unit.hpp"
#include <vector>

class MapLayerDoom : public MapLayer
{
	public:
		MapLayerDoom();
		~MapLayerDoom();
		MapLayerDoom( const MapLayerDoom& ) = delete;
		MapLayerDoom( MapLayerDoom&& ) = delete;
		MapLayerDoom& operator=( const MapLayerDoom& ) = delete;
		MapLayerDoom& operator=( MapLayerDoom&& ) = delete;
		void render( const Camera& camera ) override;
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites ) override;

	private:
		static constexpr int NUMBER_OF_COLOR_CHANNELS = 4;
		static constexpr int PIXEL_SIZE = sizeof( Uint8 ) * NUMBER_OF_COLOR_CHANNELS;
		static constexpr int RAY_MAX = Unit::WINDOW_WIDTH_PIXELS;
		static constexpr double RAY_MAX_D = ( double )( RAY_MAX );
		static constexpr int SCREEN_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS - 32;
		static constexpr double SCREEN_HEIGHT_D = ( double )( SCREEN_HEIGHT );

		struct TextureSlice
		{
			sdl2::SDLRect position;
			int texture_index;
		};

		struct Item
		{
			sdl2::SDLRect position;
			sdl2::SDLRect source;
			double distance;
			int type;
		};

		static bool sortItems( const Item& lhs, const Item& rhs );

		SDL_Texture* floor_and_ceiling_;
		std::vector<Item> items_;
		sdl2::SDLRect texture_source_;
		double wall_distances_[ RAY_MAX ];
		TextureSlice wall_items_[ RAY_MAX ];
		Uint8 floor_and_ceiling_pixels_[ SCREEN_HEIGHT ][ RAY_MAX * NUMBER_OF_COLOR_CHANNELS ];
		Uint8 floor_graphics_[ Unit::PIXELS_PER_BLOCK ][ Unit::PIXELS_PER_BLOCK * NUMBER_OF_COLOR_CHANNELS ];
		Uint8 ceiling_graphics_[ Unit::PIXELS_PER_BLOCK ][ Unit::PIXELS_PER_BLOCK * NUMBER_OF_COLOR_CHANNELS ];
};
