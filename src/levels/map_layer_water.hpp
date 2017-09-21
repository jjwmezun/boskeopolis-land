#pragma once

class Camera;

class Sprite;

#include "animated_graphics.hpp"
#include "counter.hpp"
#include "map_layer.hpp"
#include "unit.hpp"

class MapLayerWater : public MapLayer
{
	public:
		MapLayerWater( int y_blocks, bool rising );

		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) override;
		void render( const Camera& camera ) const override;
		void interact( Sprite& sprite, Health& health ) override;

	private:
		static constexpr Uint8 ALPHA = 128;
		static constexpr int COLOR = 4;

		AnimatedGraphics surface_;
		Counter x_offset_;
		TimerRepeat animation_speed_;
		int move_speed_;
		int y_;
		const bool rising_;
};