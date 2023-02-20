#ifndef MAP_LAYER_RAIN_H
#define MAP_LAYER_RAIN_H

#include <array>
#include "config.hpp"
#include "map_layer.hpp"
#include "renderer.hpp"
#include <vector>

namespace BSL
{
    class MapLayerRain : public MapLayer
    {
        public:
            MapLayerRain( unsigned int rain_start, unsigned int rain_max );
            void update( Level & level, const Game & game, float dt ) override;
            void init( const Game & game ) override;

        private:
            static constexpr int NUM_O_DROPS = WINDOW_WIDTH_PIXELS / 2;
            std::array<RectGraphic, NUM_O_DROPS> drops_;
            std::array<bool, NUM_O_DROPS> active_;
            unsigned int drop_showing_;
            std::vector<float> x_available_;
            float rain_start_;
            float rain_range_;
    };
}

#endif // MAP_LAYER_RAIN_H