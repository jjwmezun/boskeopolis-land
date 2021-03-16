#pragma once

#include "map_layer_lava.hpp"

class MapLayerLavaRise final : public MapLayer
{
	public:
        MapLayerLavaRise( int y, int rise_speed, Unit::Layer layer_position = DEFAULT_POSITION );
        ~MapLayerLavaRise();
        MapLayerLavaRise( const MapLayerLavaRise& ) = delete;
        MapLayerLavaRise( MapLayerLavaRise&& ) = delete;
        MapLayerLavaRise& operator= ( const MapLayerLavaRise& ) = delete;
        MapLayerLavaRise& operator= ( MapLayerLavaRise&& ) = delete;

        void update( LevelState& level_state ) override;
        void render( const Camera& camera ) override;
        void interact( Sprite& sprite, LevelState& level_state ) override;

    private:
        int rise_speed_;
        MapLayerLava layer_;
        char scratch_[ 3000 ];
};
