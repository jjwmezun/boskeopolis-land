#pragma once

class Camera;
class Graphics;
class Sprite;

#include "animated_graphics.h"
#include "counter.h"
#include "map_layer.h"
#include "unit.h"

class MapLayerWater : public MapLayer
{
	public:
		MapLayerWater( int y_blocks );

		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera ) override;
		void render( Graphics& graphics, Camera& camera ) const override;
		void interact( Sprite& sprite ) override;


	private:
		static constexpr int MOVE_SPEED = 500;
		int y_;
		AnimatedGraphics surface_;
		const SpriteGraphics body_;
		Counter x_offset_;
		TimerRepeat move_speed_;
};