#pragma once

class Health;
class MapLayerWater;
class Sprite;

class WaterLayerComponent
{
	public:
		virtual void update( MapLayerWater& water );
		virtual void interact( MapLayerWater& water, Sprite& sprite, Health& health ) = 0;
};