#include "block_component_hurt.hpp"
#include "sprite.hpp"

BlockComponentHurt::BlockComponentHurt() {};

BlockComponentHurt::~BlockComponentHurt() {};

void BlockComponentHurt::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	sprite.hurt();
};