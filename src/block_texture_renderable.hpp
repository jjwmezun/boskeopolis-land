#pragma once

#include "renderable.hpp"
#include "texture_box.hpp"

class LevelState;

class BlockTextureRenderable final : public Renderable
{
    public:
        BlockTextureRenderable( int layer, const LevelState& level_state );
        ~BlockTextureRenderable();
        BlockTextureRenderable( BlockTextureRenderable&& t ) = delete;
        BlockTextureRenderable& operator=( BlockTextureRenderable&& t );
        BlockTextureRenderable( const BlockTextureRenderable& ) = delete;
        BlockTextureRenderable& operator=( const BlockTextureRenderable& ) = delete;
        void render( const LevelState& level_state ) const override;
        TextureBox texture_;
};