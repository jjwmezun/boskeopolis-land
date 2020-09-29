#include "block_component_layer.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"

BlockComponentLayer::BlockComponentLayer( Unit::Layer layer ) : layer_ ( layer ) {};
BlockComponentLayer::~BlockComponentLayer() {};

void BlockComponentLayer::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	sprite.changeRenderableLayer( level_state, layer_ );
};
