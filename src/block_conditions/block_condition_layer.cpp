#include "block_condition_layer.hpp"
#include "sprite.hpp"

BlockConditionLayer::BlockConditionLayer( int layer ) : layer_ ( layer ) {};

BlockConditionLayer::~BlockConditionLayer() {};

bool BlockConditionLayer::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return sprite.layer_ == layer_;
};
