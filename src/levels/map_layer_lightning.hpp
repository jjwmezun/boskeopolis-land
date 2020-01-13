#pragma once

#include "map_layer.hpp"
#include <SDL2/SDL.h>

class BlockSystem;
class Camera;
class EventSystem;
class Health;
class Map;
class Sprite;
class SpriteSystem;

class MapLayerLightning : public MapLayer
{
	public:
        enum class State
        {
            STAGNANT,
            LIGHTENING,
            DARKENING
        };
        MapLayerLightning();
		~MapLayerLightning();
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites ) override;
		void render( const Camera& camera ) override;

    private:
        State state_;
        int timer_;
        int alpha_;
        int next_;
};
