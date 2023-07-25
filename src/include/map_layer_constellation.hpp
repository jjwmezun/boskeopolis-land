#ifndef MAP_LAYER_CONSTELLATION_H
#define MAP_LAYER_CONSTELLATION_H

#include "map_layer.hpp"

namespace BSL
{
    class MapLayerConstellation : public MapLayer
    {
        public:
            MapLayerConstellation( unsigned int width, unsigned int height, float scroll_speed );
            void update( Level & level, const Game & game, float dt ) override;
            void init( const Game & game ) override;

        private:
            unsigned int width_;
            unsigned int height_;
            float scroll_speed_;
            unsigned int graphics_;
    };
}

#endif // MAP_LAYER_CONSTELLATION_H