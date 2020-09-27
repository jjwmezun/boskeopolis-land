#pragma once

#include "renderable.hpp"

class LevelState;

class MapLayerRenderable final : public Renderable
{
    public:
        MapLayerRenderable( int id );
        ~MapLayerRenderable();
        void render( const LevelState& level_state ) const override;
        int id_;
};