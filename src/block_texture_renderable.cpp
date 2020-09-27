#include "block_texture_renderable.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include "level_state.hpp"
#include "render.hpp"

BlockTextureRenderable::BlockTextureRenderable( int layer, const LevelState& level_state )
:
    Renderable( Type::BLOCK ),
    texture_ ( level_state.currentMap().widthPixels(), level_state.currentMap().heightPixels() )
{
    texture_.init();
    texture_.startDrawing();
    level_state.blocks().renderLayerAllBlocks( level_state, layer );
    texture_.endDrawing();
};

BlockTextureRenderable::~BlockTextureRenderable()
{
    texture_.destroy();
};

void BlockTextureRenderable::render( const LevelState& level_state ) const
{
    const Camera& camera = level_state.camera();
    Render::renderRenderBox
    (
        texture_.getTexture(),
        {
            camera.x(),
            camera.y(),
            camera.width(),
            camera.height()
        },
        {
            0,
            0,
            camera.width(),
            camera.height()
        }
    );
};