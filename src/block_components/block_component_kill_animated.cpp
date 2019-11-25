#include "block_component_kill_animated.hpp"
#include "sprite.hpp"

BlockComponentKillAnimated::BlockComponentKillAnimated() {};

BlockComponentKillAnimated::~BlockComponentKillAnimated() {};

void BlockComponentKillAnimated::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	sprite.kill();
};