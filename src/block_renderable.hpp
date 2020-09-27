#pragma once

#include "renderable.hpp"

class LevelState;

class BlockRenderable final : public Renderable
{
    public:
        BlockRenderable( int layer );
        ~BlockRenderable();
        void render( const LevelState& level_state ) const override;
        int layer_;
};