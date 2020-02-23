#pragma once

#include <array>
#include "map_layer.hpp"
#include "mezun_sdl2.hpp"
#include "player_doom_sprite.hpp"
#include "timer_repeat_t.hpp"
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
		void update( LevelState& level_state ) override;

	private:
		static constexpr int NUMBER_OF_COLOR_CHANNELS = 4;
		static constexpr int PIXEL_SIZE = sizeof( Uint8 ) * NUMBER_OF_COLOR_CHANNELS;
		static constexpr int RAY_MAX = Unit::WINDOW_WIDTH_PIXELS;
		static constexpr double RAY_MAX_D = ( double )( RAY_MAX );
		static constexpr int SCREEN_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS - 32;
		static constexpr double SCREEN_HEIGHT_D = ( double )( SCREEN_HEIGHT );
		static constexpr int NUMBER_OF_ITEM_TYPES = 6;

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

		struct ItemInfo
		{
			double x;
			double y;
			int type;
		};

		bool hero_shooting_;
		TimerRepeatT<32> animation_timer_;
		const PlayerDoomSprite* hero_;
		SDL_Texture* floor_and_ceiling_;
		SDL_Texture* map_;
		std::vector<ItemInfo> item_info_;
		std::vector<Item> items_;
		int item_frames_[ NUMBER_OF_ITEM_TYPES ];
		sdl2::SDLRect texture_source_;
		const sdl2::SDLRect render_screen_;
		const sdl2::SDLRect map_src_;
		const sdl2::SDLRect map_dest_;
		sdl2::SDLRect hand_src_;
		const sdl2::SDLRect hand_dest_;
		const sdl2::SDLRect map_bars_[ 4 ];
		const int hand_frames_[ 25 ];
		double wall_distances_[ RAY_MAX ];
		TextureSlice wall_items_[ RAY_MAX ];
		Uint8 floor_and_ceiling_pixels_[ SCREEN_HEIGHT ][ RAY_MAX * NUMBER_OF_COLOR_CHANNELS ];
		Uint8 floor_graphics_[ Unit::PIXELS_PER_BLOCK ][ Unit::PIXELS_PER_BLOCK * NUMBER_OF_COLOR_CHANNELS ];
		Uint8 ceiling_graphics_[ Unit::PIXELS_PER_BLOCK ][ Unit::PIXELS_PER_BLOCK * NUMBER_OF_COLOR_CHANNELS ];

		inline bool testFloorAndCeilingNotSetup() const;
		inline void setupFloorAndCeiling();
		void updateAnimation();
		static bool sortItems( const Item& lhs, const Item& rhs );
};
