#ifndef MAP_LAYER_PALETTE_CHANGE_H
#define MAP_LAYER_PALETTE_CHANGE_H

#include "map_layer.hpp"

namespace BSL
{
    class MapLayerPaletteChange : public MapLayer
    {
        public:
            MapLayerPaletteChange( unsigned int start, unsigned int max );
            void update( Level & level, const Game & game, float dt ) override;
            void init( const Game & game ) override;

        private:
            float start_;
            float range_;
    };
}

#endif // MAP_LAYER_PALETTE_CHANGE_H