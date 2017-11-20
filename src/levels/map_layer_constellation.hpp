#pragma once

#include "map_layer.hpp"
#include "mezun_sdl2.hpp"
#include <vector>

enum class StarType
{
	NO_STAR = -1,
	SMALL_STAR = 0,
	MEDIUM_STAR = 1,
	BIG_STAR = 2,
	BRIGHT_SMALL_STAR = 3,
	BRIGHT_MEDIUM_STAR = 4,
	BRIGHT_BIG_STAR = 5,
	MOON_TOP_LEFT = 6,
	MOON_TOP_RIGHT = 7,
	MOON_BOTTOM_LEFT = 8,
	MOON_BOTTOM_RIGHT = 9,
};

class MapLayerConstellation : public MapLayer
{
	public:
		MapLayerConstellation
		(
			int map_width_blocks,
			int map_height_blocks
		);
		~MapLayerConstellation();
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) override;
		void render( const Camera& camera ) override;

	private:
		std::vector<StarType> star_pattern_;
		sdl2::SDLRect src_;
		double scroll_speed_;
		SDL_Texture* texture_;
		int height_;
		bool rendered_;

		void renderTexture();
		void formTexture();
		void testValues() const;
};