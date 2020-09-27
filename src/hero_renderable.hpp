#pragma once

#include "renderable.hpp"

class LevelState;

class HeroRenderable final : public Renderable
{
    public:
        HeroRenderable();
        ~HeroRenderable();
        void render( const LevelState& level_state ) const override;
};