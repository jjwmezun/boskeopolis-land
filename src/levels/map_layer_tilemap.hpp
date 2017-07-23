#pragma once

#include "block.hpp"
#include "map_layer.hpp"
#include <vector>

class MapLayerTilemap : public MapLayer
{
	public:
		MapLayerTilemap( const std::vector<int>& tiles, int map_width, int map_height );
		~MapLayerTilemap();
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera ) override;
		void render( Camera& camera ) const override;

	private:
		static constexpr int CAMERA_PADDING = 4;

		const std::vector<int> tiles_;
		const int width_;
		const int height_;
		std::vector<Block> blocks_;
		
		int nOfXY( int x, int y ) const;
};