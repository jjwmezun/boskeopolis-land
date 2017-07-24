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
		MapLayerWater( int y_blocks );

		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera ) override;
		void render( Camera& camera ) const override;
		void interact( Sprite& sprite, Health& health ) override;

	private:
		static constexpr Uint8 ALPHA = 128;
		static constexpr int COLOR = 4;
		static constexpr int MOVE_SPEED = 500;

		int y_;
		AnimatedGraphics surface_;
		Counter x_offset_;
		TimerRepeat animation_speed_;
};