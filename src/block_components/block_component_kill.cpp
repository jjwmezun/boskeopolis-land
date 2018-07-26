#include "block_component_kill.hpp"
#include "sprite.hpp"

BlockComponentKill::BlockComponentKill() {};

BlockComponentKill::~BlockComponentKill() {};

void BlockComponentKill::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	sprite.killNoAnimation();
};