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
	sdl2::SDLRect src_;
	sdl2::SDLRect dest_;
	sdl2::SDLRect body_;
	ComponentGroup components_;
	const std::string texture_;
	int y_;
	const int color_;
	int x_offset_;
	int timer_;
	Uint8 alpha_;

	static constexpr int WIDTH = 432;
	static constexpr int HEIGHT = 14;
	static constexpr int NUM_O_FRAMES = 4;
	static constexpr int MAX_FRAME = HEIGHT * NUM_O_FRAMES;

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

	private:
	void updateGFX( const Camera& camera );
	void updateComponents();
	void updateEventMovement( EventSystem& events );
};