#pragma once

#include "map_layer.hpp"
#include <vector>

class MapLayerRain : public MapLayer
{
	public:
        friend class MapLayerRainGradual;
		MapLayerRain( Unit::Layer layer_position = DEFAULT_POSITION, int number_of_drops = MAX_DROPS );
		~MapLayerRain();
		void update( LevelState& level_state ) override;
		void render( const Camera& camera ) override;

        static constexpr int MAX_DROPS = 200;

        struct RainDrop
        {
            sdl2::SDLRect rect;
            int color;
            int speed;
        };

    private:
        void generateDrop( int i );
        void removeDrop( int i );

        int number_of_drops_;
        bool rain_map_[ MAX_DROPS ];
        RainDrop drops_[ MAX_DROPS ];
};
