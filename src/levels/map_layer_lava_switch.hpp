#pragma once

#include "map_layer_lava.hpp"

class MapLayerLavaSwitch final : public MapLayer
{
	public:
        MapLayerLavaSwitch( int y_off_blocks, int y_on_blocks, int x_block_when_lava_rises_forever );
        ~MapLayerLavaSwitch();
        MapLayerLavaSwitch( const MapLayerLavaSwitch& ) = delete;
        MapLayerLavaSwitch( MapLayerLavaSwitch&& ) = delete;
        MapLayerLavaSwitch& operator= ( const MapLayerLavaSwitch& ) = delete;
        MapLayerLavaSwitch& operator= ( MapLayerLavaSwitch&& ) = delete;

        void update( LevelState& level_state ) override;
        void render( const Camera& camera ) override;
        void interact( Sprite& sprite, LevelState& level_state ) override;

    private:
        int y_off_;
        int y_on_;
        int x_point_when_lava_rises_forever_;
        MapLayerLava layer_;
        char scratch_[ 3000 ];
};
