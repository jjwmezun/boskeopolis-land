#pragma once

#include "block.hpp"
#include "renderable.hpp"
#include "texture_box.hpp"
#include <vector>

class LevelState;

class BlockLayer : public Renderable
{
    public:
        enum class RenderType
        {
            NORMAL,
            OFFSCREEN
        };

        BlockLayer( int position, RenderType render_type );
        ~BlockLayer() override;
        BlockLayer( BlockLayer&& );
        virtual void render( const LevelState& level_state ) const override;

        int position_;
        RenderType render_type_;
        std::vector<Block> blocks_;
        TextureBox texture_;
};