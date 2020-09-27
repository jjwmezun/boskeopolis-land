#include "block_renderable.hpp"
#include "block_system.hpp"
#include "level_state.hpp"

BlockRenderable::BlockRenderable( int layer ) : Renderable( Type::BLOCK ), layer_ ( layer ) {};
BlockRenderable::~BlockRenderable() {};

void BlockRenderable::render( const LevelState& level_state ) const
{
    level_state.blocks().renderLayer( level_state, layer_ );
};