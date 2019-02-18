#include "block_component_push_down.hpp"
#include "sprite.hpp"

BlockComponentPushDown::BlockComponentPushDown( int force ) : force_ ( force ) {};

BlockComponentPushDown::~BlockComponentPushDown() {};

void BlockComponentPushDown::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	sprite.hit_box_.y += force_;
};
