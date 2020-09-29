#pragma once

#include "block.hpp"
#include "map_layer.hpp"
#include <vector>

class MapLayerTilemap final : public MapLayer
{
	public:
		enum class FadeType
		{
			DONT_FADE,
			NOT_FADING,
			FADING
		};

		MapLayerTilemap( const std::vector<int>& tiles, int map_width, int map_height, bool fade = false, Unit::Layer layer_position = DEFAULT_POSITION );
		~MapLayerTilemap();
		void update( LevelState& level_state ) override;
		void render( const Camera& camera ) override;
		void interact( Sprite& sprite, LevelState& level_state ) override;

	private:
		static constexpr int CAMERA_PADDING = 4;

		std::vector<Block> blocks_;
		const std::vector<int> tiles_;
		const int width_;
		const int height_;
		FadeType fade_type_;
		Uint8 alpha_;

		int nOfXY( int x, int y ) const;
};
