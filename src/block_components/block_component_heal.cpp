#include "block_component_heal.hpp"
#include "health.hpp"

BlockComponentHeal::BlockComponentHeal() {};

BlockComponentHeal::~BlockComponentHeal() {};

void BlockComponentHeal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	health.heal();
};
