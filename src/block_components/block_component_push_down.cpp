#include "block_component_push_down.hpp"
#include "sprite.hpp"

BlockComponentPushDown::BlockComponentPushDown( int force ) : force_ ( force ) {};

BlockComponentPushDown::~BlockComponentPushDown() {};

void BlockComponentPushDown::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	sprite.hit_box_.y += force_;
};
