#include "block_component_climbable.hpp"
#include "sprite.hpp"

BlockComponentClimbable::BlockComponentClimbable() {};

BlockComponentClimbable::~BlockComponentClimbable() {};

void BlockComponentClimbable::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	sprite.touching_ladder_ = true;
};
