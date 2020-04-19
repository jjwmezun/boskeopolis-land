#pragma once

#include "map_layer.hpp"

class MapLayerLava final : public MapLayer
{
	public:
        MapLayerLava( int y_blocks );
        ~MapLayerLava();
        MapLayerLava( const MapLayerLava& ) = delete;
        MapLayerLava( MapLayerLava&& ) = delete;
        MapLayerLava& operator= ( const MapLayerLava& ) = delete;
        MapLayerLava& operator= ( MapLayerLava&& ) = delete;

        void update( LevelState& level_state ) override;
        void render( const Camera& camera ) override;
        void interact( Sprite& sprite, LevelState& level_state ) override;
        void setY( int new_value );
        int getY() const;

    private:
        int y_;
        int animation_timer_;
        int animation_frame_;
        char scratch_[ 3000 ];
};
