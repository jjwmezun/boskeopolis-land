#pragma once

#include "renderable.hpp"

class LevelState;

class SpriteRenderable : public Renderable
{
    public:
        SpriteRenderable( int sprite_id );
        ~SpriteRenderable();
        void render( const LevelState& level_state ) const override;
        int sprite_id_;
};