#pragma once

#include "map_layer.h"
#include "unit.h"
#include <vector>

class MapLayerConstellation : public MapLayer
{
	public:
		MapLayerConstellation
		(
			int map_width_blocks,
			int map_height_blocks,
			double scroll_speed_x = .1,
			double scroll_speed_y = .1
		);

		~MapLayerConstellation();

		void render( Camera& camera ) const override;

	private:
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
		static constexpr int BRIGHT_STAR_MODIFIER = 3;

		static constexpr int TILE_SIZE = Unit::PIXELS_PER_MINIBLOCK;
		static constexpr int STAR_CHANCE = 25;
		static constexpr int BIG_STAR_CHANCE = 10;
		static constexpr int MEDIUM_STAR_CHANCE = 25;
		static constexpr int BRIGHT_STAR_CHANCE = 25;

		// KEEP THESE MEMBERS IN THIS ORDER FOR CONSTRUCTOR INITIALIZER:
		const std::string texture_;
		const double scroll_speed_x_;
		const double scroll_speed_y_;
		const int height_;
		const int width_;
		const std::vector<StarType> star_pattern_;

		const std::vector<StarType> generateStarPattern() const;
		const int calculateWidth( int map_width_blocks ) const;
		const int calculateHeight( int map_height_blocks ) const;
		int indexFromXAndY( int x, int y ) const;
};