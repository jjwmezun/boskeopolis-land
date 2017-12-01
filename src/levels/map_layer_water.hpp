#pragma once

class Camera;

class Sprite;

#include "animated_graphics.hpp"
#include "counter.hpp"
#include "map_layer.hpp"
#include "unit.hpp"
#include <vector>
#include "water_layer_component.hpp"

using ComponentGroup = std::vector<WaterLayerComponent*>;

class MapLayerWater : public MapLayer
{
	public:

	AnimatedGraphics surface_;
	Counter x_offset_;
	TimerRepeat animation_speed_;
	ComponentGroup components_;
	int y_;
	int color_;

	static MapLayerWater* makeNormalWater( int y_blocks );
	static MapLayerWater* makeRisingWater( int y_blocks );
	static MapLayerWater* makeSludgeWater( int y_blocks );

	MapLayerWater( int y_blocks, ComponentGroup components, std::string gfx = "bg/animated_water.png", Uint8 alpha = 128, int color = 4 );
	~MapLayerWater();
	MapLayerWater( const MapLayerWater& ) = delete;
	MapLayerWater( MapLayerWater&& ) = delete;
	MapLayerWater& operator= ( const MapLayerWater& ) = delete;
	MapLayerWater& operator= ( MapLayerWater&& ) = delete;
	void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) override;
	void render( const Camera& camera ) override;
	void interact( Sprite& sprite, Health& health ) override;
};