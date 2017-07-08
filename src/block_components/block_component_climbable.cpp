#include "block_component_climbable.h"
#include "sprite.h"

BlockComponentClimbable::BlockComponentClimbable() {};

BlockComponentClimbable::~BlockComponentClimbable() {};

void BlockComponentClimbable::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	sprite.touching_ladder_ = true;
};