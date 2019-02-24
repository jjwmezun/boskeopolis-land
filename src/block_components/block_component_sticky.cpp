#include "block_component_sticky.hpp"
#include "sprite.hpp"

BlockComponentSticky::BlockComponentSticky() {};

BlockComponentSticky::~BlockComponentSticky() {};

void BlockComponentSticky::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	sprite.vx_ /= 1.8;
};
