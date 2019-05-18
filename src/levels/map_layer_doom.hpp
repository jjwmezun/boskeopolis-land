#pragma once

#include <array>
#include "counter_t.hpp"
#include "map_layer.hpp"
#include "mezun_sdl2.hpp"
#include "timer_repeat_t.hpp"
#include "unit.hpp"
#include <vector>

class MapLayerDoom : public MapLayer
{
	public:
		MapLayerDoom();
		~MapLayerDoom();
		void render( const Camera& camera ) override;
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites ) override;

	private:
		static constexpr int RAY_MAX = Unit::WINDOW_WIDTH_PIXELS;
		static constexpr double RAY_MAX_D = ( double )( RAY_MAX );
		SDL_Texture* floor_and_ceiling_;
		std::vector<sdl2::SDLRect> items_;
		std::vector<int> item_types_;
		TimerRepeatT<> animation_timer_;
		sdl2::SDLRect wall_src_;
		sdl2::SDLRect item_src_;
		std::array<int, RAY_MAX> colors_;
		std::array<int, RAY_MAX> texture_coordinate_;
		std::array<sdl2::SDLRect, RAY_MAX> lines_;
		Uint8 floor_and_ceiling_pixels_[ Unit::WINDOW_HEIGHT_PIXELS - 32 ][ RAY_MAX * 4 ];
		int floor_and_ceiling_x_[ Unit::WINDOW_HEIGHT_PIXELS - 32 ][ RAY_MAX ];
		int floor_and_ceiling_y_[ Unit::WINDOW_HEIGHT_PIXELS - 32 ][ RAY_MAX ];
};
