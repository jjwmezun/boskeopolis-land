#include "block_component_climbable.hpp"
#include "sprite.hpp"

BlockComponentClimbable::BlockComponentClimbable() {};

BlockComponentClimbable::~BlockComponentClimbable() {};

void BlockComponentClimbable::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	sprite.touching_ladder_ = true;
};
