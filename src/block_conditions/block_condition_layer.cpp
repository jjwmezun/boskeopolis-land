#include "block_condition_layer.hpp"
#include "sprite.hpp"

BlockConditionLayer::BlockConditionLayer( Unit::Layer layer ) : layer_ ( layer ) {};

BlockConditionLayer::~BlockConditionLayer() {};

bool BlockConditionLayer::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return sprite.layer_ == layer_;
};
