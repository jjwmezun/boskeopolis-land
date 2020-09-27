#pragma once

#include "renderable.hpp"

class LevelState;

class SpriteRenderable : public Renderable
{
    public:
        SpriteRenderable( int id );
        ~SpriteRenderable();
        void render( const LevelState& level_state ) const override;
        int id_;
};