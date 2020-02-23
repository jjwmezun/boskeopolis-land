#include "block_component_layer.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"

BlockComponentLayer::BlockComponentLayer( int layer ) : layer_ ( layer ) {};
BlockComponentLayer::~BlockComponentLayer() {};

void BlockComponentLayer::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	sprite.layer_ = layer_;
	sprite.graphics_->priority_ = layer_ == 2;
};
